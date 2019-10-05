#include <string>
#include "Item.h"
#include "Room.h"

Room::Room(std::string name, Item items[])
{
	DisplayName = name;
	*Items = *items;
}
