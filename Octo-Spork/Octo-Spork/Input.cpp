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
		inst.Direction = DetermineDirection(input);
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
	else {
		return FUNCTION_NONE;
	}
}

Direction Input::DetermineDirection(std::string input)
{
	if (input.find("left")) {
		return DIRECTION_LEFT;
	}
	else if (input.find("right")) {
		return DIRECTION_RIGHT;
	}
	else if (input.find("forward") || input.find("front")) {
		return DIRECTION_FORWARD;
	}
	else if (input.find("backward") || input.find("back")) {
		return DIRECTION_BACKWARD;
	}
	else {
		return DIRECTION_NONE;
	}
}

std::string Input::DetermineGoal(std::string input)
{
	int n = 2;
	std::istringstream iss(input);
	while (n-- > 0 && (iss >> input));
	return input;
}