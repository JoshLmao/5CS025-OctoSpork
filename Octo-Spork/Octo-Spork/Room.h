#pragma once
#include <string>
#include <array>

#include "Item.h"

struct Room
{
public:
	std::string Name;
	std::array<std::string, 3> Exits;
	Item RoomItem;

	Room();
	Room(std::string name);

	void SetItem(Item item);

	std::string GetDescription();

private:
	int GetRndIndex(int max);
};