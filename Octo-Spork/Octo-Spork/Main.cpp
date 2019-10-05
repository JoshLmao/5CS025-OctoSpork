#include <iostream>
#include <string>
#include <locale>
#include "Item.h"
#include "Room.h"

using namespace std;

void DisplayInfo(string msg)
{
	std::cout << msg << std::endl;
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

/* Gets the user's input and returns it */
string GetInput()
{
	string input;
	cin >> input;
	return input;
}

void initBuilding()
{
	Room startRoom("Hallway", NULL);

	Item items[] = { Item("Mouse"), Item("Bell") };
	Room mi034("MI034", items);
}

int main(int arc, char* argv[])
{
	DisplayInfo("You are stood in a hallway. There are two other hallways. Both in front of you to your left and to your right");
	DisplayInfo("What will you do?");
	
	string expected[] = { "Yes", "No" };
	string input = GetExpectedInput(expected);
	DisplayInfo("Resp: " + input);

	return 0;
}