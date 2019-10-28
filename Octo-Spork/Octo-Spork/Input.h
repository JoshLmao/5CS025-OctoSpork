#pragma once
#include <string>
#include "Function.h"

class Input
{
public:
	struct Instruction {
		Function Function;
		std::string Goal;
	};

	static Instruction ReadUser();
	static std::string GetInput();

private:
	static Function DetermineFunction(std::string input);
	static std::string DetermineGoal(std::string input);
};