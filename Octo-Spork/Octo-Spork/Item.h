#pragma once
#include <string>

class Item
{
public:
	std::string Name;

	Item();
	Item(std::string name);

	void Use();
	std::string GetDescription();

private:
	int GetRndIndex(int max);
};