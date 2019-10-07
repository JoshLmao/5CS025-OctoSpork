#include <string>
#include "Item.h"
#include "Room.h"

Room::Room()
{

}

Room::Room(std::string name) : Interactable::Interactable(name)
{
	Name = name;
}

void Room::SetItem(Item item)
{
	RoomItem = item;
}