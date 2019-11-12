#pragma once

#include <string>
#include "Colors.h"

enum Colors {
	COLORS_BLACK = 30,
	COLORS_RED = 31,
	COLORS_GREEN = 32,
	COLORS_YELLOW = 33,
	COLORS_BLUE = 34,
	COLORS_MAGENTA = 35,
	COLORS_CYAN = 36,
	COLORS_LIGHT_GRAY = 37,
	COLORS_DARK_GRAY = 38,
	COLORS_WHITE = 39,
};

class Utils
{
public:
	/* Converts a string to lower case */
	static std::string ToLower(std::string s);
	/* Compares two strings and returns true if they are the same */
	static bool ToLowerCompare(std::string a, std::string b);
	/* Gets a color from the enum */
	static std::string GetColor(Colors color);
};