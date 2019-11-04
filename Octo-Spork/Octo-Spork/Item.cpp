#include <string>

#include "Item.h"

Item::Item() 
{
	m_name = "";
}

Item::Item(std::string name)
{
	m_name = name;
}

std::string Item::GetName()
{
	return m_name;
}
