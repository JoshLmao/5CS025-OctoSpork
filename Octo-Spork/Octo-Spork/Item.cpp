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

	SetName(name);
}


int Item::GetRndIndex(int max)
{
	return rand() % max;
}

std::string Item::GetName()
{
	return m_name;
}

void Item::SetName(std::string name)
{
	m_name = name;
}

