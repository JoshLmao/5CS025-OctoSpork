#include <iostream>
#include <sstream>

#include "Input.h"

/* Parses the user's input and returns a set of instructions of where the user wants to go */
Input::Instruction Input::ReadUser()
{
	bool validInput = false;
	Instruction inst;
	while (!validInput)
	{
		std::string input = GetInput();
		inst.Function = DetermineFunction(input);
		if (inst.Function == FUNCTION_USE || inst.Function == FUNCTION_WALK) {
			inst.Goal = DetermineGoal(input);
		}

		validInput = inst.Function != FUNCTION_NONE;
	}
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
	if (input.find("walk")) {
		return FUNCTION_WALK;
	}
	else if (input.find("use")) {
		return FUNCTION_USE;
	}
	else if (input.find("enter")) {
		return FUNCTION_ENTER;
	}
	else if (input.find("view inventory")) {
		return FUNCTION_VIEW_INVENTORY;
	}
	else if (input.find("talk")) {
		return FUNCTION_TALK;
	}
	else {
		return FUNCTION_NONE;
	}
}

std::string Input::DetermineGoal(std::string input)
{
	int n = 2;
	std::istringstream iss(input);
	while (n-- > 0 && (iss >> input));
	return input;
}