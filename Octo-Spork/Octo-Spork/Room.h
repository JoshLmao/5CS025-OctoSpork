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
	
	Room();
	Room(std::string name, std::string desc);

	/* Gets a pointer to the item for this room */
	Item* GetItem();
	/*Sets the item for this room*/
	void SetItem(Item* item);

	/* Gets an exit from it's index */
	std::string GetExit(int index);
	/* Gets the total number of exits */
	int GetExitsSize();
	/* Sets the exits for this room 
	Requires a string array of room names */
	void SetExits(std::string* exits, int length);

	/* Sets the NPC for this room, limited to only one*/
	void SetNPC(NPC* npc);
	/* Gets a pointer to the NPC*/
	NPC* GetNPC();

	/*Gets the print out room description for the room*/
	std::string GetDescription();
private:
	std::string m_description;

	/* Pointer to array of exits */
	std::string* m_exits;
	/* Length of exits array */
	int m_exitsSize;

	Item* m_roomItem;
	NPC* m_roomNPC;
};