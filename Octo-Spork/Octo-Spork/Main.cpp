#include <iostream>
#include <string>
#include <sstream>
#include <locale>
#include <vector>
#include <thread>
#include <chrono>

#include "Function.h"
#include "Direction.h"
#include "Item.h"
#include "Room.h"
#include "Input.h"
#include "Utils.h"
#include "Inventory.h"

void DisplayInfo(std::string message);
void InitBuilding();
void InfoBuffer(int count = 1);
void PlayGame();
void UpdateState(Input::Instruction usrInstruction);
int FindRoomIndex(std::string roomName);
void PrintRoomInfo(Room & r);
void PrintInventory();
void Sleep(int ms);
void DisposeGame();

const int ROOM_COUNT = 11;

/// All rooms containing their items
static std::vector<Room*> AllRooms;
/// The user's inventory
Inventory m_userInventory;
/// Current index of the room the user is in
int m_roomIndex = 0;
/// Is the user currently playing the game
bool m_playingGame = false;
NPC* m_currentNPCPtr = nullptr;

int main(int arc, char* argv[])
{
	// Initialize all rooms, items before starting any gameplay
	InitBuilding();

	PlayGame();
	return 0;
}

/* Begin playing Octo-Spork, main entry */
void PlayGame()
{
	m_playingGame = true;
	DisplayInfo("Welcome to Octo-Spork!");
	DisplayInfo("Created by Josh Shepherd (1700471)");
	InfoBuffer(2);

	Sleep(1000);

	// Introduction
	DisplayInfo(" - Introduction - ");
	Sleep(2000);
	DisplayInfo("You awaken on the floor of a cold, damp and dilapidated structure. You're feeling dazed, a little confused with a slight pain in the back of your head");
	Sleep(4000);
	DisplayInfo("As you stand up, you notice you're stood inside a massive, abandoned facility. Scanning the room, you see a sign that says 'Main Hall'");
	Sleep(4000);
	DisplayInfo("You also notice there are three ghostly figures floating up and down behind you at the entrance. Maybe they can shed some light on how I got here and why?");

	InfoBuffer(2);
	
	PrintRoomInfo(*AllRooms[m_roomIndex]);
	InfoBuffer();
	while (m_playingGame)
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

	DisplayInfo("Thanks for playing! Hope you enjoyed the journey");
}

/* Creates the buildings rooms, items, etc */
void InitBuilding()
{
	std::vector<std::string> exits;

	// Floor 1
	exits = { "North Hallway", "Staircase" };
	Room* mainHall = new Room("Main Hall", "Where I woke up. There's a massive sign on the wall with it's name", exits);
	mainHall->AddItem(new Item("Example Item"));

	NPCConfig zeusConfig = NPCConfig();
	zeusConfig.Name = "Spirit of Zeus";
	zeusConfig.RequiredItemName = "Lightning Stone";
	NPC* zeusNPC = new NPC(zeusConfig);
	mainHall->AddNPC(zeusNPC);

	NPCConfig freyaConfig = NPCConfig();
	freyaConfig.Name = "Sprit of Freya";
	freyaConfig.RequiredItemName = "Earth Stone";
	NPC* freyaNPC = new NPC(freyaConfig);
	mainHall->AddNPC(freyaNPC);

	NPCConfig balderConfig = NPCConfig();
	balderConfig.Name = "Spirit of Balder";
	balderConfig.RequiredItemName = "Light Stone";
	NPC* balderNPC = new NPC(balderConfig);
	mainHall->AddNPC(balderNPC);

	exits = { "Main Hall", "East Hallway", "MI034" };
	Room* nHallway = new Room("North Hallway", "Long hallway", exits);
	
	exits = { "North Hallway", "MI035" };
	Room* eHallway = new Room("East Hallway", "Hidden in the back of the complex", exits);

	// Floor 1 - Rooms
	// MI034
	exits = { "North Hallway", "MI035" };
	Room* mi034 = new Room("MI034", "A quiet little room tucked away at the back of the facility", exits);
	mi034->AddItem(new Item("Computer Mouse"));
	mi034->AddItem(new Item("Broken Headset"));
	
	NPCConfig govrConfig = NPCConfig();
	govrConfig.Name = "Ghost of VR";
	govrConfig.RequiredItemName = "Example Item";
	govrConfig.Greeting = "Greetings wanderer. What brings you along my path? Is it the promise of loot or to free me from this cursed room?";
	govrConfig.StandardResponse = "Please, wanderer. I have been here for 500 years now. My death is the only thing that bounds me to the realm. Can you help free me?";
	govrConfig.ExcessiveResponse = "Do you wish to torture me by returning so often? Help me or leave me be.";
	govrConfig.HasItemResponse = "Ah, finally! Now I may rest in peace. I am eternally greateful. Thank you";
	govrConfig.CompleteResponse = "You have served me well. Now go, let me play VR";
	govrConfig.IncorrectItemResponse = "What is this? Don't waste my time with irrelevent things.";
	govrConfig.ExcessiveLimitCount = 15;

	NPC* ghostVR = new NPC(govrConfig);
	mi034->AddNPC(ghostVR);

	// MI035
	exits = { "East Hallway" };
	Room* mi035 = new Room("MI035", "Description", exits);
	mi035->AddItem(new Item("Ominous Candle"));
	mi035->AddItem(new Item("VR Controller"));

	// Floor 1 to 2 Staircase 
	exits = { "Main Hall", "F2 Landing" };
	Room* staircase = new Room("Staircase", "", exits);

	// Floor 2
	exits = { "Staircase", "F2 North Hallway", "F2 West Hallway" };
	Room* f2Landing = new Room("F2 Landing", "", exits);

	exits = { "F2 East Hallway", "F2 Landing", "MI102c", "F2 West Hallway" };
	Room* f2nHallway = new Room("F2 North Hallway", "", exits);

	exits = { "F2 North Hallway", "F2 East Hallway" };
	Room* f2eHallway = new Room("F2 East Hallway", "", exits);

	exits = { "F2 Landing", "F2 North Hallway", "F2 Easy Hallway", "Staircase" };
	Room* f2wHallway = new Room("F2 West Hallway", "", exits);

	// Floor 2 - Rooms
	exits = { "F2 Landing" };
	Room* mi102a = new Room("MI102a", "", exits);

	Room* mi102b = new Room("MI102b", "", exits);
	Item* Ostone = new Item("The O stone");
	mi102b->AddItem(Ostone);

	exits = { "F2 North Hallway" };
	Room* mi102c = new Room("MI102c", "", exits);

	// Add all rooms to Vector
	AllRooms.push_back(mainHall);
	AllRooms.push_back(nHallway);
	AllRooms.push_back(eHallway);
	AllRooms.push_back(mi034);
	AllRooms.push_back(mi035);
	AllRooms.push_back(staircase);
	AllRooms.push_back(f2Landing);
	AllRooms.push_back(f2nHallway);
	AllRooms.push_back(mi102a);
	AllRooms.push_back(mi102b);
	AllRooms.push_back(mi102c);
}

/* Adds an amount of new line char's specified in 'count' */
void InfoBuffer(int count)
{
	std::string el;
	for (int i = 0; i < count; i++) {
		el += "\n";
	}
	std::cout << el;
}

/* Displays a message to the console with a specific format */
inline void DisplayInfo(std::string message)
{
	std::cout << "> " << message << std::endl;
}

/* Sleeps the main thread for the specified amount of milliseconds */
inline void Sleep(int ms)
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void UpdateState(Input::Instruction usrInstruction)
{
	if (usrInstruction.Function == Function::FUNCTION_HELP) 
	{
		// User asking for help
		DisplayInfo("Help Commands");
		DisplayInfo("help - Lists all available commands to use");
		DisplayInfo("enter 'ROOM' - Used to move from one room to another");
		DisplayInfo("talk 'NPC' - Talks to an NPC");
		DisplayInfo("give 'ITEM' - Attempts to give an item to the current NPC in the room");
		DisplayInfo("take 'ITEM' - Takes an item that is inside the current room");
		DisplayInfo("view inventory - Lists your current items you're holding");
		DisplayInfo("view room - Lists the current room's information");
		DisplayInfo("exit - Quit the game");

		InfoBuffer();
		DisplayInfo("Tip: Remember you can talk to NPCs multiple times for more information");
	}
	else if (usrInstruction.Function == Function::FUNCTION_TALK) 
	{
		// User talking to an NPC
		m_currentNPCPtr = AllRooms[m_roomIndex]->GetNPC(usrInstruction.Goal);
		
		if (m_currentNPCPtr == nullptr) {
			DisplayInfo("There is no one here named '" + usrInstruction.Goal + "'.");
		}
		else {
			std::string speech = m_currentNPCPtr->GetSpeech();
			DisplayInfo(speech);
		}
	}
	else if (usrInstruction.Function == Function::FUNCTION_ENTER)
	{
		// User enters new room
		for (int i = 0; i < AllRooms[m_roomIndex]->GetExitsSize(); i++)
		{
			std::string exitLower = Utils::ToLower(AllRooms[m_roomIndex]->GetExit(i));
			std::string goalLower = Utils::ToLower(usrInstruction.Goal);
			if (exitLower == goalLower)
			{
				m_roomIndex = FindRoomIndex(goalLower);
				break;
			}
		}

		// Reset current talking to NPC since user changed room
		if (m_currentNPCPtr != nullptr)
			m_currentNPCPtr = nullptr;

		// Print information of new room
		PrintRoomInfo(*AllRooms[m_roomIndex]);
	}
	else if (usrInstruction.Function == Function::FUNCTION_VIEW_INVENTORY) 
	{
		// User viewing their inventory
		DisplayInfo("You look through your belongings...");
		PrintInventory();
	}
	else if (usrInstruction.Function == Function::FUNCTION_VIEW_ROOM) {
		// Print the room info to the user
		PrintRoomInfo(*AllRooms[m_roomIndex]);
	}
	else if (usrInstruction.Function == Function::FUNCTION_GIVE) 
	{
		// User wants to give an item to the current NPC
		if (m_currentNPCPtr == nullptr) {
			DisplayInfo("You try to give '" + usrInstruction.Goal + "'. However, you only hear a loud clunk as the item fall to the floor.");
			DisplayInfo("Maybe you should make sure someone is there to give the item to first?");
			return;
		}

		bool result = m_currentNPCPtr->GiveItem(usrInstruction.Goal);
		DisplayInfo(m_currentNPCPtr->GetSpeech());
	}
	else if (usrInstruction.Function == Function::FUNCTION_TAKE) 
	{
		// User picks up an item. 
		int size = AllRooms[m_roomIndex]->GetItemsSize();
		if (size <= 0) {
			DisplayInfo("There are no items here.");
			return;
		}

		bool contains = AllRooms[m_roomIndex]->ItemsContains(usrInstruction.Goal);
		if (contains) {
			Item* itmPtr = AllRooms[m_roomIndex]->RemoveItem(usrInstruction.Goal);
			m_userInventory.AddItem(itmPtr);
			DisplayInfo("You pick up " + itmPtr->GetName());
		} else {
			DisplayInfo("You tried to pick up '" + usrInstruction.Goal + "' but couldn\'t find it");
		}
	}
	else if (usrInstruction.Function == Function::FUNCTION_EXIT) 
	{
		// User wants to exit game. Confirm and quit if so
		DisplayInfo("Are you sure you wish to quit? Type 'exit' again to confirm");
		Input::Instruction inst = Input::ReadUser();
		if (inst.Function == FUNCTION_EXIT)
			m_playingGame = false;
	}
}

int FindRoomIndex(std::string roomName)
{
	for (int i = 0; i < AllRooms.size(); i++)
	{
		std::string roomLower = Utils::ToLower(AllRooms[i]->Name);
		if (roomLower == roomName)
		{
			return i;
		}
	}
}

/*	Prints out room information in the format:
	> Room Name
	> - - - - 
	> Items: Itm1, Itm2
	> Exits: Exit1, Exit2
	> NPCs: NPC1, NPC2
	*/
void PrintRoomInfo(Room & r)
{
	std::string out = "";

	std::string endl = "\n";
	out += "> " + r.Name + endl;
	out += "> - - - - - - " + endl;
	
	// Append items if there are any
	int itmsSize = r.GetItemsSize();
	if (itmsSize > 0) {
		out += "> Items: ";
		for (unsigned int i = 0; i < itmsSize; i++) {
			out += r.GetItem(i)->GetName();
			if (i < itmsSize - 1) {
				out += ", ";
			}
		}
		out += endl;
	}
		
	// Append exists if exists
	int exitsSize = r.GetExitsSize();
	if (exitsSize > 0) {
		out += "> Exits: ";
		for (int i = 0; i < exitsSize; i++) {
			out += r.GetExit(i);
			if (i < exitsSize - 1)
				out += ", ";
		}
		out += endl;
	}

	// Append NPCs if exists
	int npcsSize= r.GetNPCSize();
	if (npcsSize > 0) {
		out += "> NPCs: ";
		for (int i = 0; i < npcsSize; i++) {
			out += r.GetNPC(i)->GetName();
			if (i < npcsSize - 1)
				out += ", ";
		}
	}

	std::cout << out;
}

/*	Prints out the inventory of the user */
void PrintInventory()
{
	std::string msg = "";
	for (unsigned int i = 0; i < m_userInventory.GetSize(); i++) {
		std::string itemName = m_userInventory.GetItem(i)->GetName();
		if (itemName != "") {
			msg += itemName;
			if (i < m_userInventory.GetMaxSize() - 2)
				msg += ",";
		}
	}

	if (msg == "")
		msg = "You have no items in your inventory.";
	DisplayInfo(msg);
}

void DisposeGame()
{
	for (int i = 0; i < AllRooms.size(); i++)
	{
		delete AllRooms[i];
	}
}