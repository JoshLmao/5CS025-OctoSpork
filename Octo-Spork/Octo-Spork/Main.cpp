#include <iostream>
#include <string>
#include <sstream>
#include <locale>
#include <array>

#include "Function.h"
#include "Direction.h"
#include "Item.h"
#include "Room.h"
#include "Input.h"

void DisplayInfo(std::string message);
void InitBuilding();
void InfoBuffer(int count = 1);
void PlayGame();
void UpdateState(Input::Instruction usrInstruction);
int FindRoomIndex(std::string roomName);
void PrintRoomInfo(Room r);

const int ROOM_COUNT = 10;

//static Room ActiveRoom;
static std::array<Room, ROOM_COUNT> AllRooms;

int m_roomIndex = 0;

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
	DisplayInfo("Welcome to Octo-Spork!");
	DisplayInfo("Created by Josh Shepherd (1700471)");
	InfoBuffer(2);

	// Introduction
	DisplayInfo(" - Introduction - ");
	DisplayInfo("You awaken on the floor of a cold, damp and dilapidated structure. You're feeling dazed, a little confused with a slight pain in the back of your head");
	DisplayInfo("As you stand up, you notice you're stood inside a massive, abandoned facility. Scanning the room, you see a sign that says 'Main Hall'");
	InfoBuffer();

	while (true)
	{
		PrintRoomInfo(AllRooms[m_roomIndex]);
		DisplayInfo("What will you do?");
		Input::Instruction inst = Input::ReadUser();
		UpdateState(inst);
	}
}

/* Creates the buildings rooms, items, etc */
void InitBuilding()
{
	Room mainHall = Room("Main-Hall", "Where I woke up. There's a massive sign on the wall with it's name");
	mainHall.Exits = { "North-Hallway", "West-Hallway", "Staircase" };

	Room nHallway = Room("North-Hallway", "Long hallway");
	nHallway.Exits = { "Main-Hall", "MI034" };

	Room eHallway = Room("East-Hallway", "Hidden in the back of the complex");
	eHallway.Exits = { "North-Hallway", "MI035" };

	Room mi034 = Room("MI034", "A room");
	Item mouse = Item("Mouse");
	mi034.SetItem(mouse);

	Room mi035 = Room("MI035", "Description");
	Item candle = Item("Candle");
	mi035.SetItem(candle);

	AllRooms = {
		mainHall, nHallway, eHallway, mi034, mi035
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

void UpdateState(Input::Instruction usrInstruction)
{
	for (int i = 0; i < AllRooms[m_roomIndex].Exits.size(); i++)
	{
		if (AllRooms[m_roomIndex].Exits[i] == usrInstruction.Goal)
		{
			m_roomIndex = FindRoomIndex(usrInstruction.Goal);
			break;
		}
	}
}

int FindRoomIndex(std::string roomName)
{
	for (int i = 0; i < AllRooms.size(); i++)
	{
		if (AllRooms[i].Name == roomName)
		{
			return i;
		}
	}
}

void PrintRoomInfo(Room r)
{
	std::cout << r.GetInfo();
}