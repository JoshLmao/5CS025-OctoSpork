#pragma once
#include <string>

#include "Item.h"
#include "Interactable.h"

struct Room : public Interactable
{
	Item RoomItem;

public:
	Room();
	Room(std::string name);

	void SetItem(Item item);
};