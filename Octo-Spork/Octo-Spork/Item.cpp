#include <string>

#include "Item.h"

Item::Item() 
{
	m_name = "";
	m_description = "";
}

Item::Item(std::string name, std::string desc)
{
	m_name = name;
	m_description = desc;
}

std::string Item::GetName()
{
	return m_name;
}

std::string Item::Examine()
{
	return m_description;
}