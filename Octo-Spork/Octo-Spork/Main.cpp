#include <iostream>

int main(int arc, char* argv[])
{
	std::cout << "Hello, world!" << std::endl;

	std::cout << "Press the enter key to exit";
	std::cin.ignore(std::cin.rdbuf()->in_avail() + 1);
}