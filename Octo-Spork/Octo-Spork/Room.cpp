#include <string>
#include <cstdlib>
#include <ctime>
#include <array>

#include "Item.h"
#include "Room.h"

Room::Room() 
{
}

Room::Room(std::string name, std::string desc)
{
	Name = name;
	m_description = desc;
}

void Room::SetItem(Item item)
{
	RoomItem = item;
}

std::string Room::GetDescription()
{
	return m_description;
}

std::string Room::GetInfo()
{
	std::string header = "> " + Name;
	std::string separator = "> - - - - - - ";
	std::string desc = "> " + m_description;
	std::string items = "> Items: " + RoomItem.Name;
	std::string exits = "> Exits: ";
	for (int i = 0; i < Exits.size(); i++) {
		exits += Exits[i];
		if (i < Exits.size() - 1)
			exits += ", ";
	}
	std::string npcs = "> NPCs: " + RoomNPC.GetName();
	return header + "\n" + separator + "\n" + desc + "\n" + items + "\n" + exits + "\n" + npcs + "\n";
}

void Room::SetNPC(NPC npc) 
{
	RoomNPC = npc;
}