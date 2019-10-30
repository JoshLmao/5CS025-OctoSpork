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
	m_exits = nullptr;
	m_exitsSize = 0;
}

Room::Room(std::string name, std::string desc)
{
	Name = name;
	m_description = desc;

	m_roomItem = nullptr;
	m_roomNPC = nullptr;
	m_exits = nullptr;
	m_exitsSize = 0;
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
	return m_exits[index];
}

int Room::GetExitsSize()
{
	return m_exitsSize;
}

void Room::SetExits(std::string* exitsPtr, int length)
{
	m_exits = exitsPtr;
	m_exitsSize = length;
}
