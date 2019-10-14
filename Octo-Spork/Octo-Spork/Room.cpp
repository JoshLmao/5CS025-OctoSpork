#include <string>
#include <cstdlib>
#include <ctime>
#include <array>

#include "Item.h"
#include "Room.h"

Room::Room() 
{
	srand((unsigned int)time(0));
}

Room::Room(std::string name)
{
	Name = name;
	srand((unsigned int)time(0));
}

void Room::SetItem(Item item)
{
	RoomItem = item;
}

std::string Room::GetDescription()
{
	std::array<std::string, 3> phrases = {
		"You walk into ",
		"You stroll on into ",
		"You entered. Infront of you is a massive sign that says ",
	};

	return phrases[GetRndIndex((unsigned int)phrases.size())] + Name;
}

int Room::GetRndIndex(int max) 
{
	return rand() % max;
}