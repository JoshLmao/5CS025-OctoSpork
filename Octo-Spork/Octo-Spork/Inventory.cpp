#include "Inventory.h"
#include "Utils.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

Item* Inventory::GetItem(int index)
{
	if (index > -1 && index < m_inventory.size()) {
		return m_inventory[index];
	} else {
		return nullptr;
	}
}

bool Inventory::Contains(std::string itmName) 
{
	std::vector<Item*>::iterator itr;
	for (itr = m_inventory.begin(); itr < m_inventory.end(); itr++) {
		bool r = Utils::ToLowerCompare((*itr)->GetName(), itmName);
		if (r) {
			return true;
		}
	}
	return false;
}

void Inventory::AddItem(Item* itmPtr)
{
	if (m_inventory.size() > USER_INVENTORY_SIZE - 1) {
		return;
	}
	m_inventory.push_back(itmPtr);
}

Item* Inventory::RemoveItem(std::string itmName) 
{
	int index = -1;
	std::vector<Item*>::iterator itr;
	for (itr = m_inventory.begin(); itr < m_inventory.end(); itr++) {
		bool r = Utils::ToLowerCompare((*itr)->GetName(), itmName);
		if (r) {
			index = itr - m_inventory.begin();
		}
	}
	if (index >= 0) {
		Item* itmPtr = m_inventory[index];
		m_inventory.erase(m_inventory.begin() + index);
		return itmPtr;
	}
	return nullptr;
}

int Inventory::GetSize()
{
	return m_inventory.size();
}

int Inventory::GetMaxSize()
{
	return USER_INVENTORY_SIZE;
}