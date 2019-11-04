#pragma once
#include <vector>

#include "Item.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	Item* GetItem(int index);
	void AddItem(Item* itmPtr);
	
	int GetSize();
	int GetMaxSize();

private:
	const int USER_INVENTORY_SIZE = 2;

	std::vector<Item*> m_inventory;
};