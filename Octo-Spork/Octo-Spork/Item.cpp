#include <string>

#include "Item.h"
#include "Interactable.h"

Item::Item() { }

Item::Item(std::string name) : Interactable::Interactable(name)
{
	Name = name;
}

void Item::Use()
{

}