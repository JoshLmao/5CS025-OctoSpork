#pragma once
#include <string>
#include <vector>

#include "Item.h"
#include "NPC.h"

struct Room
{
public:
	std::string Name;
	std::vector<std::string> Exits;
	
	Room();
	Room(std::string name, std::string desc, std::vector<std::string> exits);

	/* Gets a pointer to the item for this room */
	Item* GetItem();
	/*Sets the item for this room*/
	void SetItem(Item* item);

	/* Gets an exit from it's index */
	std::string GetExit(int index);

	/* Sets the NPC for this room, limited to only one*/
	void SetNPC(NPC* npc);
	/* Gets a pointer to the NPC*/
	NPC* GetNPC();

	/*Gets the print out room description for the room*/
	std::string GetDescription();
private:
	std::string m_description;

	Item* m_roomItem;
	NPC* m_roomNPC;
};