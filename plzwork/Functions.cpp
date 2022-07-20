#include "Functions.h"
#include <iostream>

void Functions::Log(const std::string& text, std::string str, bool endl)
{
	std::cout << '[' << str << ']' << ' ' << text;
	if (endl)
		std::cout << std::endl;
}