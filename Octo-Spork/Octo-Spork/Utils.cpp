#include "Utils.h"

#include <string>
#include <locale>

std::string Utils::ToLower(std::string s)
{
	std::string lower = "";
	std::locale loc;
	for (int i = 0; i < s.length(); i++)
		lower += std::tolower(s[i], loc);
	return lower;
}

bool Utils::ToLowerCompare(std::string a, std::string b) 
{
	return ToLower(a) == ToLower(b);
}