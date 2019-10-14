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
void InfoBuffer();
void PlayGame();
void UpdateState(Input::Instruction usrInstruction);
int FindRoomIndex(std::string roomName);

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
	InfoBuffer();

	DisplayInfo("You awaken on the floor of a cold, damp and dilapidated structure. You're feeling dazed, a little confused with a slight pain in the back of your head");
	DisplayInfo("As you stand up, you notice you're stood in the middle of a hallway.");
	DisplayInfo("There are two other hallways. Both in front of you to your left and to your right");
	DisplayInfo("What will you do?");
	
	while (true)
	{
		Input::Instruction inst = Input::ReadUser();
		UpdateState(inst);
	}
}

/* Creates the buildings rooms, items, etc */
void InitBuilding()
{
	Room mi034 = Room("MI034");
	Item mouse = Item("Mouse");
	mi034.SetItem(mouse);

	Room mi035 = Room("MI035");
	Item candle = Item("Candle");
	mi035.SetItem(candle);

	Room hallway = Room("Main Hall");
	hallway.Exits = { "North Hallway", "West Hallway", "Staircase" };

	Room nHallway = Room("North Hallway");
	nHallway.Exits = { "Main Hall", "MI034" };

	Room eHallway = Room("East Hallway");
	eHallway.Exits = { "North Hallway", "MI035" };

	AllRooms = {
		hallway, mi034, mi035
	};
}

inline void InfoBuffer()
{
	std::cout << std::endl << std::endl;
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