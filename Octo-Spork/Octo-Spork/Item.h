#pragma once
#include <string>

class Item
{
public:
	Item();
	Item(std::string name);

	std::string GetName();

private:
	std::string m_name;
};