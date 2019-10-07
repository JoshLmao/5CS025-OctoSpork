#pragma once
#include <string>

#include "Interactable.h"

class Item : public Interactable
{
public:

	Item();
	Item(std::string name);

	void Use();
};