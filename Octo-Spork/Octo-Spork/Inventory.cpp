#include "Inventory.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

Item* Inventory::GetItem(int index)
{
	if (index > m_inventory.size()) {
		return nullptr;
	}
	return m_inventory[index];
}

void Inventory::AddItem(Item* itmPtr)
{
	if (m_inventory.size() > USER_INVENTORY_SIZE - 1) {
		return;
	}
	m_inventory.push_back(itmPtr);
}

int Inventory::GetSize()
{
	return m_inventory.size();
}

int Inventory::GetMaxSize()
{
	return USER_INVENTORY_SIZE;
}