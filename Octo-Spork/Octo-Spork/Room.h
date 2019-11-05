#pragma once
#include <string>
#include <vector>

#include "Item.h"
#include "NPC.h"

struct Room
{
public:
	std::string Name;

	Room();
	Room(std::string name, std::string desc, std::vector<std::string> exits);
	Room(std::string name, std::string desc, std::vector<std::string> exits, std::vector<Item*> items);
	~Room();

	/* Gets an item inside this room, from its index */
	Item* GetItem(int index);
	/* Adds an item for this room*/
	void AddItem(Item* item);
	/* Removes an item from this room */
	Item* RemoveItem(std::string itmName);
	/* Does the Items in this room contain this item */
	bool ItemsContains(std::string name);
	/* Returns the amount of items inside this room */
	int GetItemsSize();

	/* Gets an exit from it's index */
	std::string GetExit(int index);
	/* Gets the amount of exits set for the room */
	int GetExitsSize();

	/* Sets the NPC for this room, limited to only one*/
	void SetNPC(NPC* npc);
	/* Gets a pointer to the NPC*/
	NPC* GetNPC();

	/*Gets the print out room description for the room*/
	std::string GetDescription();

private:
	/* Standard description of this room */
	std::string m_description;
	/* All exits available in this room */
	std::vector<std::string> m_exits;
	/* Items inside the room */
	std::vector<Item*> m_items;
	/* NPC inside the room */
	NPC* m_npc;
};