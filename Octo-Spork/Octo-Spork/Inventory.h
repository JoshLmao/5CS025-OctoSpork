#pragma once
#include <vector>

#include "Item.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	/* Gets an item from player inventory with it's index. Returns nullptr if out of bounds */
	Item* GetItem(int index);
	/* Tries to get an item in the user inventory, returning nullptr if not found */
	Item* TryGetItem(std::string name);
	/* Checks if the inventory contains an item name */
	bool Contains(std::string itmName);
	/* Adds an item to the player's inventory. Returns if it could be added (true) or not (false) */
	bool AddItem(Item* itmPtr);
	/*	Adds an item to the player's inventory with the option to forceAdd to their inventory, regardless of inventory size.
		For example, if it's a necessary item to progress with story */
	void AddItem(Item* itmPtr, bool forceAdd);
	/* Removes an item from the player's inventory and returns it */
	Item* RemoveItem(std::string itmName);

	/* Gets current size of the player's inventory */
	int GetSize();
	/* Gets the max size of the player's inventory, maximum amount of items the player can hold*/
	int GetMaxSize();
	/* Can the player pick up another item? */
	bool CanAddItem();

	/* Disposes of all Items inside the inventory */
	void Dispose();

private:
	const int USER_INVENTORY_SIZE = 2;
	/* The user's inventory */
	std::vector<Item*> m_inventory;
};