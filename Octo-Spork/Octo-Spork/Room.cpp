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

	m_exits = std::vector<std::string>(0);
	m_items = std::vector<Item*>(0);
	m_npcs = std::vector<NPC*>(0);
}

Room::Room(std::string name, std::string desc, std::vector<std::string> exits)
{
	Name = name;
	m_description = desc;

	m_exits = std::vector<std::string>(exits.size());
	for (int i = 0; i < exits.size(); i++)
		m_exits[i] = exits[i];

	m_items = std::vector<Item*>(0);
	m_npcs = std::vector<NPC*>(0);
}

Room::Room(std::string name, std::string desc, std::vector<std::string> exits, std::vector<Item*> items)
{
	Name = name;
	m_description = desc;

	m_exits = std::vector<std::string>(exits.size());
	for (int i = 0; i < exits.size(); i++)
		m_exits[i] = exits[i];

	m_items = items;
	m_npcs = std::vector<NPC*>(0);
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
	if (index > -1 && index < m_items.size())
		return m_items[index];
	else
		return nullptr;
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

NPC* Room::GetNPC(int index)
{
	if (index > -1 && index < m_npcs.size())
		return m_npcs[index];
	else
		return nullptr;
}

NPC* Room::GetNPC(std::string npcName)
{
	int index = -1;
	for (unsigned int i = 0; i < GetNPCSize(); i++) {
		NPC* npc = GetNPC(i);
		if (Utils::ToLowerCompare(npcName, npc->GetName())) {
			index = i;
		}
	}

	if (index > -1 && index < GetNPCSize())
		return m_npcs[index];
	else
		return nullptr;

}

void Room::AddNPC(NPC* npc) 
{
	m_npcs.push_back(npc);
}

int Room::GetNPCSize()
{
	return m_npcs.size();
}

std::string Room::GetExit(int index)
{
	if (index > -1 && index < m_exits.size())
		return m_exits[index];
	else
		return "";
}

int Room::GetExitsSize()
{
	return m_exits.size();
}


void Room::AddExit(std::string exitName)
{
	m_exits.push_back(exitName);
}