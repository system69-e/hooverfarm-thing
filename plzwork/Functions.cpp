#include "Functions.h"
#include <iostream>

void Functions::Log(const std::string& text, string str)
{
	switch (str)
	{
	case "Autorestart"_str:
		std::cout << "[Autorestart]" << text;
		break;
	case "Init"_str:
		std::cout << "[Init]" << text;
		break;
	default:
		throw "";
	}
}