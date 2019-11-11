#pragma once
#include <string>

class Item
{
public:
	Item();
	Item(std::string name, std::string desc);

	std::string GetName();
	std::string Examine();

private:
	std::string m_name;
	std::string m_description;
};