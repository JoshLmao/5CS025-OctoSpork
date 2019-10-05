#pragma once
#include <string>
#include "Item.h"

class Room
{
public:
	std::string DisplayName;
	Item Items[];

	Room(std::string name, Item items[]);
};