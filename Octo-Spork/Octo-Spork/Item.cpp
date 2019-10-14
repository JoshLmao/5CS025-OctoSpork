#include <string>
#include <cstdlib>
#include <ctime>
#include <array>

#include "Item.h"

Item::Item() 
{
	srand((unsigned int)time(0));
}

Item::Item(std::string name)
{
	srand((unsigned int)time(0));

	Name = name;
}

void Item::Use()
{

}

std::string Item::GetDescription()
{
	std::array<std::string, 3> phrases = {
		"It's a shiny ",
		"You pick up ",
		"Ah. It's only a ",
	};

	return phrases[GetRndIndex((unsigned int)phrases.size())] + Name;
}

int Item::GetRndIndex(int max)
{
	return rand() % max;
}