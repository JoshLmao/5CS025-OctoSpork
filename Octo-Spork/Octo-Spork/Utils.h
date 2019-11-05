#pragma once

#include <string>

class Utils
{
public:
	/* Converts a string to lower case */
	static std::string ToLower(std::string s);
	/* Compares two strings and returns true if they are the same */
	static bool ToLowerCompare(std::string a, std::string b);
};