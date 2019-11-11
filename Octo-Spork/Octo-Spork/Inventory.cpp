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

bool Inventory::AddItem(Item* itmPtr)
{
	if (m_inventory.size() > USER_INVENTORY_SIZE - 1) {
		return false;
	}
	m_inventory.push_back(itmPtr);
	return true;
}

void Inventory::AddItem(Item* itmPtr, bool forceAdd)
{
	if (forceAdd) {
		m_inventory.push_back(itmPtr);
	}
	else {
		AddItem(itmPtr);
	}
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

void Inventory::Dispose()
{
	for (int i = 0; i < GetSize(); i++)
	{
		if (m_inventory[i] != nullptr)
			delete m_inventory[i];
	}
}

bool Inventory::CanAddItem()
{
	return GetSize() < GetMaxSize();
}

Item* Inventory::TryGetItem(std::string name)
{
	for (int i = 0; i < GetSize(); i++) {
		Item* itm = GetItem(i);
		if (Utils::ToLowerCompare(itm->GetName(), name)) {
			return itm;
		}
	}
	return nullptr;
}