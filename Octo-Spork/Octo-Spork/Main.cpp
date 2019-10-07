#include <iostream>
#include <string>
#include <locale>
#include <array>

#include "Outcomes.h"
#include "Item.h"
#include "Room.h"

using namespace std;

string GetExpectedInput(string results[]);
string GetInput();
void DisplayInfo(string message);
void InitBuilding();
void InfoBuffer();
void PlayGame();

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

	string expected[] = { "walk left", "walk right" };
	string input = GetExpectedInput(expected);
	DisplayInfo("Resp: " + input);
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
	cout << std::endl << std::endl;
}

inline void DisplayInfo(string message)
{
	std::cout << message << std::endl;
}

/* Gets the user's input and returns it */
string GetInput()
{
	string input;
	cin >> input;
	return input;
}

/* Repeatedly asks for input from the user until recieving a response that is in 'results[]' */
string GetExpectedInput(string results[])
{
	string input;
	bool hasExpected = false;
	int index = 0;
	while (!hasExpected)
	{
		cin >> input;

		index =  0;
		for (index = 0; index < results->size(); index++)
		{
			if (input == results[index]) {
				hasExpected = true;
				break;
			}
		}
		if (!hasExpected)
			DisplayInfo("Unexpect input. Try again!");
	}
	return results[index];
}
