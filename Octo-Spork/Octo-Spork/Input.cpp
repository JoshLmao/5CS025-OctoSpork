#include <iostream>
#include <sstream>


#include "Input.h"
#include "Utils.h"

/* Parses the user's input and returns a set of instructions of where the user wants to go */
Input::Instruction Input::ReadUser()
{
	bool validInput = false;
	Instruction inst;
	
	std::string input = GetInput();
	inst.Function = DetermineFunction(input);
	inst.Goal = DetermineGoal(input);

	return inst;
}

/* Gets the user's input and returns it */
std::string Input::GetInput()
{
	std::string input;
	std::getline(std::cin, input);
	return input;
}

Function Input::DetermineFunction(std::string input)
{
	// Convert to lower case for expected comparisons
	std::string lower = Utils::ToLower(input);
	if (lower.find("enter") != std::string::npos) {
		return FUNCTION_ENTER;
	}
	else if (lower.find("view inventory") != std::string::npos) {
		return FUNCTION_VIEW_INVENTORY;
	}
	else if (lower.find("talk") != std::string::npos) {
		return FUNCTION_TALK;
	}
	else if (lower.find("give") != std::string::npos) {
		return FUNCTION_GIVE;
	}
	else if (lower.find("take") != std::string::npos) {
		return FUNCTION_TAKE;
	}
	else if (lower.find("drop") != std::string::npos) {
		return FUNCTION_DROP;
	}
	else if (lower.find("help") != std::string::npos || lower.find("?") != std::string::npos) {
		return FUNCTION_HELP;
	}
	else if (lower.find("view room") != std::string::npos) {
		return FUNCTION_VIEW_ROOM;
	}
	else if (lower.find("exit") != std::string::npos) {
		return FUNCTION_EXIT;
	}
	else if (lower.find("examine") != std::string::npos) {
		return FUNCTION_EXAMINE;
	}
	else {
		return FUNCTION_NONE;
	}
}

std::string Input::DetermineGoal(std::string input)
{
	std::size_t pos = input.find(" ");
	if (pos != std::string::npos)
	{
		// + 1 to remove white space
		return input.substr(pos + 1);
	}
	else 
	{
		return "?";
	}
}