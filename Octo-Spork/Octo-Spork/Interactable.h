#pragma once
#include <string>

class Interactable 
{
public:
	std::string Name;

	Interactable();
	Interactable(std::string name);

	std::string GetDescription();
};