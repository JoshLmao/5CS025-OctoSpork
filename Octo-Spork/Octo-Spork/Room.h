#pragma once
#include <string>

#include "Item.h"

struct Room
{
public:
	std::string Name;
	Item RoomItem;

	Room();
	Room(std::string name);

	void SetItem(Item item);
	std::string GetDescription();

private:
	int GetRndIndex(int max);
};