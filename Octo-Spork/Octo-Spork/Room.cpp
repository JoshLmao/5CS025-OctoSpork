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

	m_item = nullptr;
	m_npc = nullptr;
}

Room::Room(std::string name, std::string desc, std::vector<std::string> exits)
{
	Name = name;
	m_description = desc;

	m_exits = std::vector<std::string>(exits.size());
	for (int i = 0; i < exits.size(); i++)
		m_exits[i] = exits[i];

	m_item = nullptr;
	m_npc = nullptr;
}

Room::~Room() 
{
	if (m_item != nullptr)
		delete m_item;
}

std::string Room::GetDescription()
{
	return m_description;
}

Item* Room::GetItem()
{
	return m_item;
}

void Room::SetItem(Item* item)
{
	m_item = item;
}

NPC* Room::GetNPC() 
{
	return m_npc;
}

void Room::SetNPC(NPC* npc) 
{
	m_npc = npc;
}

std::string Room::GetExit(int index)
{
	return m_exits[index];
}

int Room::GetExitsSize()
{
	return m_exits.size();
}
