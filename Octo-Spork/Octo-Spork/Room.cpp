#include <string>
#include <cstdlib>
#include <ctime>
#include <array>

#include "Item.h"
#include "Room.h"

Room::Room() 
{
	Name = "Unknown Room";
	m_description = "Unknown description";

	m_roomItem = nullptr;
	m_roomNPC = nullptr;
}

Room::Room(std::string name, std::string desc, std::vector<std::string> exits)
{
	Name = name;
	m_description = desc;

	Exits = std::vector<std::string>(exits.size());
	for (int i = 0; i < exits.size(); i++)
		Exits[i] = exits[i];

	m_roomItem = nullptr;
	m_roomNPC = nullptr;
}

std::string Room::GetDescription()
{
	return m_description;
}

Item* Room::GetItem()
{
	return m_roomItem;
}

void Room::SetItem(Item* item)
{
	m_roomItem = item;
}

NPC* Room::GetNPC() 
{
	return m_roomNPC;
}

void Room::SetNPC(NPC* npc) 
{
	m_roomNPC = npc;
}

std::string Room::GetExit(int index)
{
	return Exits[index];
}

