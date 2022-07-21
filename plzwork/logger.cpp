#include "logger.h"
#include <iostream>

void Log(const std::string& text, std::string function, bool endl)
{
	std::cout << '[' << function << ']' << ' ' << text << (endl ? '\n' : ' ');
}

void Log(const std::string& text, LogLevel level, bool endl) 
{
	std::cout << '[' << LogLevelNames[level] << ']' << ' ' << text << (endl ? '\n' : ' ');
}
