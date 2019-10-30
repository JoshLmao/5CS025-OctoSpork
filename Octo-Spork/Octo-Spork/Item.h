#pragma once
#include <string>

class Item
{
public:
	Item();
	Item(std::string name);

	std::string GetName();
	void SetName(std::string);

private:
	std::string m_name;

	int GetRndIndex(int max);
};