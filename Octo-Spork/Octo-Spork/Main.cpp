#include <iostream>
#include <string>
#include <sstream>
#include <locale>
#include <array>
#include <thread>
#include <chrono>

#include "Function.h"
#include "Direction.h"
#include "Item.h"
#include "Room.h"
#include "Input.h"
#include "Utils.h"

void DisplayInfo(std::string message);
void InitBuilding();
void InfoBuffer(int count = 1);
void PlayGame();
void UpdateState(Input::Instruction usrInstruction);
int FindRoomIndex(std::string roomName);
void PrintRoomInfo(Room r);
void Sleep(int ms);

const int ROOM_COUNT = 11;
const int USER_INVENTORY_SIZE = 4;

/// All rooms containing their items
static std::array<Room, ROOM_COUNT> AllRooms;

/// The user's inventory
Item* m_userInventory;
/// Current index of the room the user is in
int m_roomIndex = 0;

int main(int arc, char* argv[])
{
	// Initialize all rooms, items before starting any gameplay
	InitBuilding();
	// Init user's inventory
	Item inventory[USER_INVENTORY_SIZE];
	m_userInventory = inventory;
	
	PlayGame();
	return 0;
}

/* Begin playing Octo-Spork, main entry */
void PlayGame()
{
	DisplayInfo("Welcome to Octo-Spork!");
	DisplayInfo("Created by Josh Shepherd (1700471)");
	InfoBuffer(2);

	//Sleep(1000);

	// Introduction
	DisplayInfo(" - Introduction - ");
	//Sleep(2000);
	DisplayInfo("You awaken on the floor of a cold, damp and dilapidated structure. You're feeling dazed, a little confused with a slight pain in the back of your head");
	//Sleep(4000);
	DisplayInfo("As you stand up, you notice you're stood inside a massive, abandoned facility. Scanning the room, you see a sign that says 'Main Hall'");
	//Sleep(4000);

	InfoBuffer();
	
	PrintRoomInfo(AllRooms[m_roomIndex]);
	InfoBuffer();
	while (true)
	{
		DisplayInfo("What will you do?");
		Input::Instruction inst = Input::ReadUser();
		InfoBuffer();

		if (inst.Function != FUNCTION_NONE) {
			UpdateState(inst);
		}
		else {
			DisplayInfo("Invalid command. Try again");
		}
		
		InfoBuffer();
	}
}

/* Creates the buildings rooms, items, etc */
void InitBuilding()
{
	// Floor 1
	Room mainHall = Room("Main Hall", "Where I woke up. There's a massive sign on the wall with it's name");
	mainHall.Exits = { "North Hallway", "Staircase" };

	Room nHallway = Room("North Hallway", "Long hallway");
	nHallway.Exits = { "Main Hall", "East Hallway", "MI034" };

	Room eHallway = Room("East Hallway", "Hidden in the back of the complex");
	eHallway.Exits = { "North Hallway", "MI035" };

	// Floor 1 - Rooms
	// MI034
	Room mi034 = Room("MI034", "A quiet little room tucked away");
	mi034.Exits = { "North Hallway", "MI035" };
	Item mouse = Item("Mouse");
	mi034.SetItem(mouse);
	
	NPCConfig govrConfig = NPCConfig();
	govrConfig.Name = "Ghost of VR";
	govrConfig.RequiredItemName = "Valve Index";
	govrConfig.Greeting = "Greetings wanderer. What brings you along my path? Is it the promise of loot or to free me from this cursed room?";
	govrConfig.StandardResponse = "Please, wanderer. Can you free me from this room? I have been here for 500 years now. ";
	govrConfig.ExcessiveResponse = "Do you wish to torture me by returning so often?";
	govrConfig.HasItemResponse = "Ah, finally! Now I may rest in peace";
	govrConfig.CompleteResponse = "You have served me well. Now go, let me play VR";
	govrConfig.ExcessiveLimitCount = 15;

	NPC ghostVR = NPC(govrConfig);
	mi034.SetNPC(ghostVR);

	// MI035
	Room mi035 = Room("MI035", "Description");
	mi035.Exits = { "East Hallway" };
	Item candle = Item("Candle");
	mi035.SetItem(candle);

	// Floor 1 to 2 Staircase 
	Room staircase = Room("Staircase", "");
	staircase.Exits = { "Main Hall", "F2 Landing" };

	// Floor 2
	Room f2Landing = Room("F2 Landing", "");
	f2Landing.Exits = { "Staircase", "F2 North Hallway" };

	Room f2nHallway = Room("F2 North Hallway", "");
	f2nHallway.Exits = { "F2 East Hallway", "F2 Landing", "MI102" };

	Room f2eHallway = Room("F2 East Hallway", "");
	f2eHallway.Exits = { "F2 North Hallway", "F2 East Hallway" };

	// Floor 2 - Rooms
	Room mi102a = Room("MI102a", "");
	mi102a.Exits = { "F2 Landing" };

	Room mi102b = Room("MI102b", "");
	mi102b.Exits = { "F2 North Hallway" };
	mi102b.SetItem(Item("Index"));

	AllRooms = {
		mainHall, nHallway, eHallway, mi034, mi035, staircase, f2Landing, f2nHallway, f2eHallway, mi102a, mi102b
	};
}

void InfoBuffer(int count)
{
	std::string el;
	for (int i = 0; i < count; i++) {
		el += "\n";
	}
	std::cout << el;
}

inline void DisplayInfo(std::string message)
{
	std::cout << "> " << message << std::endl;
}

inline void Sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void UpdateState(Input::Instruction usrInstruction)
{
	if (usrInstruction.Function == Function::FUNCTION_USE) 
	{
		// User trying to use an Item
	}
	else if (usrInstruction.Function == Function::FUNCTION_TALK) 
	{
		// User talking to an NPC
		NPC npc = AllRooms[m_roomIndex].RoomNPC;
		std::string speech = npc.GetSpeech();
		DisplayInfo(speech);
	}
	else if (usrInstruction.Function == Function::FUNCTION_ENTER)
	{
		// User enters new room
		for (int i = 0; i < AllRooms[m_roomIndex].Exits.size(); i++)
		{
			std::string exitLower = Utils::ToLower(AllRooms[m_roomIndex].Exits[i]);
			std::string goalLower = Utils::ToLower(usrInstruction.Goal);
			if (exitLower == goalLower)
			{
				m_roomIndex = FindRoomIndex(goalLower);
				break;
			}
		}

		PrintRoomInfo(AllRooms[m_roomIndex]);
		InfoBuffer();
	}
	else if (usrInstruction.Function == Function::FUNCTION_VIEW_INVENTORY) 
	{
		// User viewing their inventory
		DisplayInfo("You look through your belongings...");

		std::string msg = "";
		for (int i = 0; i < USER_INVENTORY_SIZE; i++) {
			std::string itemName = (m_userInventory + i)->Name;
			if (itemName != "") {
				msg += itemName;
				if (i < USER_INVENTORY_SIZE - 2)
					msg += ",";
			}
		}

		if (msg == "")
			msg = "You have no items in your inventory.";
		DisplayInfo(msg);
	}
}

int FindRoomIndex(std::string roomName)
{
	for (int i = 0; i < AllRooms.size(); i++)
	{
		std::string roomLower = Utils::ToLower(AllRooms[i].Name);
		if (roomLower == roomName)
		{
			return i;
		}
	}
}

void PrintRoomInfo(Room r)
{
	std::cout << r.GetInfo();
}