#pragma once
#include <string>

class Item
{
public:
	std::string DisplayName;
	
	Item();
	Item(std::string displayName);
	std::string Use();
};