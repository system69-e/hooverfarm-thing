
#include <iostream>
#include <string>
#include <fstream>
#include "configmanagerroutine.h"

bool inputToFile(const std::string& link,
						bool FarmShinys=1,
						bool AntiCrash=1,
						bool PityMode=0,
						bool RibMode=0)
																
{
	std::ofstream file;
	file.open("hooverYBA.lua");
	
	file << "getgenv().Config = {" << std::endl;
	file << '[' << '"' << "FarmShinys" << '"' << ']' << '=' << std::boolalpha << FarmShinys <<   std::noboolalpha   << ',' << std::endl;
	file << '[' << '"' << "AntiCrash"  << '"' << ']' << '=' << std::boolalpha << AntiCrash <<    std::noboolalpha	<< ',' << std::endl;
	file << '[' << '"' << "PityMode"   << '"' << ']' << '=' << std::boolalpha << PityMode <<     std::noboolalpha	<< ',' << std::endl;
	file << '[' << '"' << "RibMode"    << '"' << ']' << '=' << std::boolalpha << RibMode <<		 std::noboolalpha	<< ',' << std::endl;
	file << '[' << '"' << "AntiLag"    << '"' << ']' << '=' << "true,"													   << std::endl;
	file << '[' << '"' << "Timing"     << '"' << ']' << '=' << "1"														   << std::endl;
	file << '}' << std::endl;


	file << "--[[YOU NEED TO BE ATLEAST LEVEL 3 FOR SHINY FARM (LEVEL 6 For RibFarm)]]--" << std::endl;
	file << "--[[NO THE SCRIPT DOES NOT SKIP ASSETS.]]--" << std::endl;

	file << "loadstring(game:HttpGet(" << '"' << link << ',' << "true))() : Activate()";

	file.close();
	return true;
}


void ConfigmanagerClass::createConfig(int input, const std::string& link)
{
	switch (input)
	{
	case 1:
		if(inputToFile(link))
		{
			std::cout << "Config created successfully" << std::endl;
		}
		else
		{
			std::cout << "Config creation failed" << std::endl;
		}
		break;
	case 2:
		if(inputToFile(link, 0, 1, 0, 0))
		{
			std::cout << "Config created successfully" << std::endl;
		}
		else
		{
			std::cout << "Config creation failed" << std::endl;
		}
	}

	//-- move the file to autoexec
	std::string command = "move hooverYBA.lua ../autoexec";
	system(command.c_str());
	system("cls");

}

bool ConfigmanagerClass::checkConfig()
{
	//-- check if file is present in ../autoexec
	std::ifstream file;
	file.open("../autoexec/hooverYBA.lua");
	if (file.is_open())
	{
		file.close();
		return true;
	}
	else
	{
		file.close();
		return false;
	}
}

void ConfigmanagerClass::configManager(std::string link)
{
	std::cout << "[1] Default config" << std::endl;
	std::cout << "[2] Item farm config" << std::endl;

	int input;
	std::cin >> input;

	createConfig(input, link);
}