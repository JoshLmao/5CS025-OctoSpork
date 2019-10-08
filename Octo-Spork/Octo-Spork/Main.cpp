#include <iostream>
#include <string>
#include <sstream>
#include <locale>
#include <array>

#include "Function.h"
#include "Direction.h"
#include "Item.h"
#include "Room.h"


struct Instruction {
	Function Function;
	Direction Direction;
	std::string GoalName;
};


std::string GetInput();
Instruction ReadUser();
void DisplayInfo(std::string message);
void InitBuilding();
void InfoBuffer();
void PlayGame();
Function DetermineFunction(std::string input);
Direction DetermineDirection(std::string input);
std::string DetermineGoal(std::string input);

static Room ActiveRoom;
static std::array<Room, 2> AllRooms;

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

	Instruction inst = ReadUser();
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

	AllRooms = {
		mi034, mi035
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

/* Gets the user's input and returns it */
std::string GetInput()
{
	std::string input;
	std::getline(std::cin, input);
	return input;
}

/* Parses the user's input and returns a set of instructions of where the user wants to go */
Instruction ReadUser()
{
	bool validInput = false;
	Instruction inst;
	while (!validInput)
	{
		std::string input = GetInput();
		inst.Function = DetermineFunction(input);
		inst.Direction = DetermineDirection(input);
		if (inst.Function == FUNCTION_USE || inst.Function == FUNCTION_WALK) {
			inst.GoalName = DetermineGoal(input);
		}

		validInput = inst.Function != FUNCTION_NONE;
	}
	return inst;
}

Function DetermineFunction(std::string input)
{
	if (input.find("walk")) {
		return FUNCTION_WALK;
	} else if (input.find("use")) {
		return FUNCTION_USE;
	} else if (input.find("enter")) {
		return FUNCTION_ENTER;
	} else {
		return FUNCTION_NONE;
	}
}

Direction DetermineDirection(std::string input)
{
	if (input.find("left")) {
		return DIRECTION_LEFT;
	} else if (input.find("right")) {
		return DIRECTION_RIGHT;
	} else if (input.find("forward") || input.find("front")) {
		return DIRECTION_FORWARD;
	} else if (input.find("backward") || input.find("back")) {
		return DIRECTION_BACKWARD;
	} else {
		return DIRECTION_NONE;
	}
}

std::string DetermineGoal(std::string input) 
{
	int n = 2;
	std::istringstream iss(input);
	while (n-- > 0 && (iss >> input));
	return input;
}