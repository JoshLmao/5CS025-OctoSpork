#pragma once
#include <string>
#include "Function.h"
#include "Direction.h"

class Input
{
public:
	struct Instruction {
		Function Function;
		Direction Direction;
		std::string Goal;
	};

	static Instruction ReadUser();
	static std::string GetInput();

private:
	static Function DetermineFunction(std::string input);
	static Direction DetermineDirection(std::string input);
	static std::string DetermineGoal(std::string input);
};