#pragma once
#include <string>

class Configmanager
{
	public:
		void createConfig(int input, const std::string& link);
		void configManager(std::string link);
		bool checkConfig();
};