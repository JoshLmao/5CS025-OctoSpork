#include <string>

#include "Interactable.h"

Interactable::Interactable()
{

}

Interactable:: Interactable(std::string name)
{
	Name = name;
}

std::string Interactable::GetDescription()
{
	return "You do something in " + Name;
}