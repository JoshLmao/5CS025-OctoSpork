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
	Room(std::string name, std::string desc);

	/*Sets the item for this room*/
	void SetItem(Item item);
	/*Gets the print out room description for the room*/
	std::string GetDescription();
	/*Returns formatted string of the room.
	In the order, Name, Description, Item(s), Exit(s)*/
	std::string GetInfo();

private:
	std::string m_description;
};