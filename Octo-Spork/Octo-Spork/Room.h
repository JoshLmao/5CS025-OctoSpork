#pragma once
#include <string>
#include <array>

#include "Item.h"
#include "NPC.h"

struct Room
{
public:
	std::string Name;
	std::array<std::string, 3> Exits;
	std::string m_description;
	Item RoomItem;
	NPC RoomNPC;

	Room();
	Room(std::string name, std::string desc);

	/*Sets the item for this room*/
	void SetItem(Item item);

	/*Gets the print out room description for the room*/
	std::string GetDescription();

	/*Returns formatted string of the room.
	In the order, Name, Description, Item(s), Exit(s)*/
	std::string GetInfo();
	/* Sets the NPC for this room, limited to only one*/
	void SetNPC(NPC npc);
};