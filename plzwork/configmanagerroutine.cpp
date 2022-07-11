
#include <iostream>
#include <string>
#include <fstream>
#include "configmanagerroutine.h"

struct Config { 
	bool FarmShinys;
	bool AntiCrash;
	bool PityMode;
	bool RibMode;
	bool AntiLag;
	float Timing;
};

const std::string configFile = "hooverYBA.lua";
const std::string autoExecPath = "../autoexec/" + configFile;

const Config configurations[] {
	{
		true, true, false, false, true, 1.0f // default config
	},
	{
		false, true, false, false, true, 1.0f // do not farm shinys
	}
};

const std::string configNames[] {
	"Default",
	"Not Items"
};

#define writeBoolOption(name) \
 '[' << '"' << #name << '"' << ']' << '=' << std::boolalpha << config.name << std::noboolalpha << ',' << std::endl;

#define writePrimitiveOption(name) \
 '[' << '"' << #name << '"' << ']' << '=' << config.name << ',' << std::endl;

bool inputToFile(const std::string& link, Config config = configurations[0])									
{
	//-- actually handles the input to the file
	std::ofstream file;
	file.open(configFile);
	
	file << "getgenv().Config = {" << std::endl;
	file << writeBoolOption(FarmShinys);
	file << writeBoolOption(AntiCrash);
	file << writeBoolOption(AntiLag);
	file << writeBoolOption(PityMode);
	file << writeBoolOption(RibMode);
	file << writePrimitiveOption(Timing);
	file << '}' << std::endl;


	file << "--[[YOU NEED TO BE ATLEAST LEVEL 3 FOR SHINY FARM (LEVEL 6 For RibFarm)]]--" << std::endl;
	file << "--[[NO THE SCRIPT DOES NOT SKIP ASSETS.]]--" << std::endl;

	file << "loadstring(game:HttpGet(" << '"' << link << ',' << "true))() : Activate()";

	file.close();
	return true;
}

void ConfigmanagerClass::createConfig(int input, const std::string& link)
{
	//-- switch to handle user's choice
	if(inputToFile(link, configurations[input - 1]))
	{
		std::cout << "Config created successfully" << std::endl;
	}
	else
	{
		std::cout << "Config creation failed" << std::endl;
	}

	//-- move the file to autoexec
	system(("move " + configFile + " " + autoExecPath).c_str());
	system("cls");
}

bool ConfigmanagerClass::checkConfig()
{
	//-- check if file is present in ../autoexec, currently unused
	std::ifstream file;
	file.open(autoExecPath);
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
	size_t configSize = sizeof(configNames) / sizeof(configNames[0]);
	for (int i = 0; i < configSize; i++)
	{
		std::cout << i + 1 << ") " << configNames[i] << " config" << std::endl;
	}

	int input;
	std::cin >> input;

	createConfig(input, link);
}