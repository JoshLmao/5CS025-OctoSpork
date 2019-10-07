#include <iostream>
#include <string>
#include <locale>
#include <array>

#include "Item.h"
#include "Room.h"

using namespace std;

string GetExpectedInput(string results[]);
string GetInput();
void DisplayInfo(string message);
void InitBuilding();

static Room ActiveRoom;
static std::array<Room, 2> AllRooms;

int main(int arc, char* argv[])
{
	InitBuilding();

	for (int i = 0; i < AllRooms.size(); i++)
	{
		std::cout << "Room: " << AllRooms[i].Name << std::endl;
		std::cout << "Room: " << AllRooms[i].GetDescription() << std::endl;
	}

	return 0;


	DisplayInfo("You are stood in a hallway. There are two other hallways. Both in front of you to your left and to your right");
	DisplayInfo("What will you do?");
	
	string expected[] = { "Yes", "No" };
	string input = GetExpectedInput(expected);
	DisplayInfo("Resp: " + input);

	return 0;
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

		index = 0;
		for (index = 0; index < sizeof(results); index++)
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
