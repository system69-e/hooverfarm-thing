#include <iostream>
#include <string>
#include <fstream>
#include "configmanagerroutine.h"

struct Config {
  float Teleport_Timing;
  bool Use_Ribcages;
  bool Farm_Shinys;
  struct {
      bool Farm_Pity;
      float Pity_Goal;
  } Pity_Config;
  bool Reduce_Crashes;
  bool Anti_Lag;
};

const std::string configFile = "hooverYBA.lua";
const std::string autoExecPath = "../autoexec/" + configFile;

const Config configurations[] {
	{
		0.5f, false, true, { false, 2.0f }, true, true // default config
	},
	{
		0.5f, false, false, { false, 2.0f }, true, true // do not farm shinys
	}
};

const std::string configNames[] {
	"Default",
	"Item farm"
};

#define option(name) "getgenv()[\"" << #name << "\"] = " << config.name << std::endl; 
#define bool_option(name) "getgenv()[\"" << #name << "\"] = " << (config.name ? "true" : "false") << std::endl;
#define named_option(name, value) "[\"" << #name << "\"] = " << config.value << ',' << std::endl;
#define named_bool_option(name, value) "[\"" << #name << "\"] = " << (config.value ? "true" : "false") << ',' << std::endl;

bool inputToFile(const std::string& link, Config config = configurations[0])									
{
	//-- actually handles the input to the file
	std::ofstream file;
	file.open(configFile);
	
	file << option(Teleport_Timing)
	file << bool_option(Use_Ribcages)
	file << bool_option(Farm_Shinys)
	file << "getgenv()[\"Pity_Config\"] = {" << std::endl;
	file << named_bool_option(Farm_Pity, Pity_Config.Farm_Pity)
	file << named_option(Pity_Goal, Pity_Config.Pity_Goal)
	file << "}" << std::endl;
	file << bool_option(Reduce_Crashes)
	file << bool_option(Anti_Lag)

	file << "--[[YOU NEED TO BE ATLEAST LEVEL 3 FOR SHINY FARM (LEVEL 6 For RibFarm)]]--" << std::endl;
	file << "--[[NO THE SCRIPT DOES NOT SKIP ASSETS.]]--" << std::endl;

	file << "loadstring(game:HttpGet(" << '"' << link << '"' << ',' << "true))() : Activate()";

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
		std::cout << "[" << i + 1 << "] " << configNames[i] << " config" << std::endl;
	}

	int input;
	std::cin >> input;

	createConfig(input, link);
}