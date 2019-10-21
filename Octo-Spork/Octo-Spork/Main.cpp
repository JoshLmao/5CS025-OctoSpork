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
		InfoBuffer();
	}
}

/* Creates the buildings rooms, items, etc */
void InitBuilding()
{
	// Floor 1
	Room mainHall = Room("Main-Hall", "Where I woke up. There's a massive sign on the wall with it's name");
	mainHall.Exits = { "North-Hallway", "West-Hallway", "Staircase" };

	Room nHallway = Room("North-Hallway", "Long hallway");
	nHallway.Exits = { "Main-Hall", "MI034" };

	Room eHallway = Room("East-Hallway", "Hidden in the back of the complex");
	eHallway.Exits = { "North-Hallway", "MI035" };

	Room sHallway = Room("South-Hallway", "");
	sHallway.Exits = { "East-Hallway", "West-Hallway" };

	Room wHallway = Room("West-Hallway", "");
	wHallway.Exits= { "South-Hallway", "Main-Hall" };

	// Floor 1 - Rooms
	Room mi034 = Room("MI034", "A room");
	Item mouse = Item("Mouse");
	mi034.SetItem(mouse);

	Room mi035 = Room("MI035", "Description");
	Item candle = Item("Candle");
	mi035.SetItem(candle);

	// Floor 1 to 2 Staircase 
	Room staircase = Room("Staircase", "");
	staircase.Exits = { "Main-Hall", "F2-Landing" };

	// Floor 2
	Room f2Landing = Room("F2-Landing", "");
	f2Landing.Exits = { "Staircase", "F2-North-Hallway" };

	Room f2nHallway = Room("F2-North-Hallway", "");
	f2nHallway.Exits = { "F2-East-Hallway", "F2-Landing", "MI102" };

	Room f2eHallway = Room("F2-East-Hallway", "");
	f2eHallway.Exits = { "F2-North-Hallway", "F2-East-Hallway" };

	Room f2sHallway = Room("F2-South-Hallway", "");
	f2sHallway.Exits = { "F2-East-Hallway", "F2-North-Hallway" };

	// Floor 2 - Rooms
	Room mi102a = Room("MI102a", "");
	mi102a.Exits = { "F2-Landing" };

	Room mi102c = Room("MI102c", "");
	mi102c.Exits = { "F2-North-Hallway" };

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