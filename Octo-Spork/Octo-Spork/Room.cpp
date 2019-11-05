#include <string>
#include <cstdlib>
#include <ctime>
#include <array>

#include "Item.h"
#include "Room.h"
#include "Utils.h"

Room::Room() 
{
	Name = "Unknown Room";
	m_description = "Unknown description";

	m_items = std::vector<Item*>(0);

	m_npc = nullptr;
}

Room::Room(std::string name, std::string desc, std::vector<std::string> exits)
{
	Name = name;
	m_description = desc;

	m_exits = std::vector<std::string>(exits.size());
	for (int i = 0; i < exits.size(); i++)
		m_exits[i] = exits[i];

	m_items = std::vector<Item*>(0);

	m_npc = nullptr;
}

Room::Room(std::string name, std::string desc, std::vector<std::string> exits, std::vector<Item*> items)
{
	Name = name;
	m_description = desc;

	m_exits = std::vector<std::string>(exits.size());
	for (int i = 0; i < exits.size(); i++)
		m_exits[i] = exits[i];

	m_items = items;
	m_npc = nullptr;
}

Room::~Room() 
{
	// Delete all Items
	for (auto i : m_items) {
		delete i;
	}
}

std::string Room::GetDescription()
{
	return m_description;
}

Item* Room::GetItem(int index)
{
	return m_items[index];
}

int Room::GetItemsSize()
{
	return m_items.size();
}

void Room::AddItem(Item* itm)
{
	m_items.push_back(itm);
}

Item* Room::RemoveItem(std::string name)
{
	int index = -1;
	std::vector<Item*>::iterator itr;
	for (itr = m_items.begin(); itr < m_items.end(); itr++) {
		bool r = Utils::ToLowerCompare((*itr)->GetName(), name);
		if (r) {
			index = itr - m_items.begin();
		}
	}
	if (index >= 0) {
		Item* itmPtr = m_items[index];
		m_items.erase(m_items.begin() + index);
		return itmPtr;
	}
	return nullptr;
}

bool Room::ItemsContains(std::string name)
{
	std::vector<Item*>::iterator itr;
	for (itr = m_items.begin(); itr < m_items.end(); itr++) {
		bool r = Utils::ToLowerCompare((*itr)->GetName(), name);
		if (r) {
			return true;
		}
	}
	return false;
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
