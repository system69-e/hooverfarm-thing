#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <vector>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

#include "terminal.h"
#include "configmanagerroutine.h"
#include "Functions.h"

struct Config
{
    float Teleport_Timing;
    bool Use_Ribcages;
    bool Farm_Shinys;
    struct {
        bool Farm_Pity;
        float Pity_Goal;
    } Pity_Config;
    bool Reduce_Crashes;
    bool Anti_Lag;
    /*
    struct {
    bool Use_Webhook;
    std::string Webhook_URL;
    } Webhook_config;
  */
};

const std::string configFile = "hooverYBA.lua";
std::string autoExecPath = "../autoexec/" + configFile;


//read config.ini
void readcfg()
{
    std::ifstream configFileStream("config.ini");
    if (!configFileStream.is_open())
    {
        std::cout << "Could not open config file" << std::endl;
        return;
    }
    std::string line;
    std::getline(configFileStream, line);

    //get the parent folder of line
	std::string parentFolder = line.substr(0, line.find_last_of("\\"));
    autoExecPath = parentFolder + "/autoexec/" + configFile;
}

Config configurations[]
{
  {
    0.5f, false, true, { false, 2.0f }, true, true  //-- default config
  },
  {
    0.5f, false, false, { false, 2.0f }, true, true //-- do not farm shinys
  },
  {
    0.5f, false, false, { true, 2.0f }, true, true  //-- farm pity only
  },

};

const std::string configNames[]
{
  "Default",
  "Item farm",
  "Pity farm only",
};

const std::string items[] {
    "Rib Cage of The Saint's Corpse",
    "Mysterious Arrow",
    "Pure Rokakaka",
    "Lucky Arrow",
    "Gold Coin",
    "Diamond",
    "DEO's Diary",
    "Stone Mask",
    "Quinton's Glove",
    "Ancient Scroll"
    "Steel Balls",
};

#define option(name) "getgenv()[\"" << #name << "\"] = " << config.name << std::endl; 
#define bool_option(name) "getgenv()[\"" << #name << "\"] = " << (config.name ? "true" : "false") << std::endl;
#define named_option(name, value) "[\"" << #name << "\"] = " << config.value << std::endl;
#define named_string_option(name, value) "[\"" << #name << "\"] = " << '"' << config.value << '"'<< std::endl;
#define named_bool_option(name, value) "[\"" << #name << "\"] = " << (config.value ? "true" : "false") << ',' << std::endl;

bool inputToFile(const std::string& link, Config config = configurations[0])
{
    Functions functions;
    if (config.Pity_Config.Farm_Pity)
    {
        goto pity;
    }
    else
    {
        goto normal;
    }

pity:
    getc(stdin); //-- skip remainin chars
read:
    std::cout << "Enter pity goal: ";

    float f;
    try
    {
        std::string buffer = "";
        char c;

        while ((c = getchar()) != '\n')
            buffer += c;

		if (std::stof(buffer) < 1.5f || std::stof(buffer) > 5.0f)
		{
			std::cout << "Invalid input" << std::endl;
			goto read;
		}

        if (buffer.empty())
            f = 2.0f;
        else f = std::stof(buffer);
    }
    catch (...)
    {
        std::cout << "Invalid input, try again." << std::endl;
        goto read;
    }

    config.Pity_Config.Pity_Goal = f;


normal:
    getc(stdin); //-- skip remaining chars
    /*
    std::cout << "Do you want to use the webhook feature? (y/n)";
    char c;
    std::cin >> c;
    switch (c)
    {
      case 'y': case 'Y':
          config.Webhook_config.Use_Webhook = true;
          break;
      case 'n': case 'N':
          config.Webhook_config.Use_Webhook = false;
          break;
      default:
            config.Webhook_config.Use_Webhook = false;
          break;
    }

    if (config.Webhook_config.Use_Webhook)
    {
      std::cout << "Enter webhook URL: ";
      std::string buffer;
      std::cin >> buffer;
      config.Webhook_config.Webhook_URL = buffer;
    }

    */
    //-- actually handles the input to the file
read_items:
    functions.Log("Items to farm:", "Config Manager", 1);
    for(int i = 0; i < sizeof(items) / sizeof(items[0]); i++)
    {
        std::cout << "     " << i + 1 << "." << items[i] << std::endl;
    }
    functions.Log("Enter the items you want to farm (example: 1,3,6): ", "Config Manager");
	
    std::string user_input;
    std::getline(std::cin, user_input);
    std::vector<int> user_choice;
    std::stringstream ss(user_input);
    int temp;
    while (ss >> temp)
    {
        user_choice.push_back(temp);
        if (ss.peek() == ',')
            ss.ignore();
    }

	//-- file creation
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
    //file << "getgenv()[\"Webhook_Config\"] = {" << std::endl;
    //file << named_bool_option(Use_Webhook, Webhook_config.Use_Webhook)
    //file << named_string_option(Webhook_URL, Webhook_config.Webhook_URL)
    //file << "}" << std::endl;


    file << "getgenv()[\"Extra_Items\"] = {" << std::endl;
	
    for (int i = 0; i < user_choice.size(); i++)
    {
        int choice = user_choice[i];
        if(choice > 0 && choice <= sizeof(items) / sizeof(items[0]))
        {
            file << '"' << items[user_choice[i] - 1] << '"' << (i == user_choice.size() - 1 ? "" : ",") << std::endl;
        }
        else 
        {
            std::cout << "Invalid input, try again." << std::endl;
            goto read_items;
        }
    }


    file << '}' << std::endl;

    file << "--[[YOU NEED TO BE ATLEAST LEVEL 3 FOR SHINY FARM (LEVEL 6 For RibFarm)]]--" << std::endl;
    file << "--[[NO THE SCRIPT DOES NOT SKIP ASSETS.]]--" << std::endl;


    file << "loadstring(game:HttpGet(" << '"' << link << '"' << ',' << " true))():Activate()" << std::endl;

    file.close();
	
    return true;
}

void ConfigmanagerClass::createConfig(int input, const std::string& link)
{
    clear();
    //-- switch to handle user's choice
    if (inputToFile(link, configurations[input - 1]))
    {
        std::cout << "Config created successfully" << std::endl;
    }
    else
    {
        std::cout << "Config creation failed" << std::endl;
    }
    //-- move the file to the autoexec folder
    fs::rename(configFile, autoExecPath);
    clear();
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

void ConfigmanagerClass::configManager(const std::string link)
{
    Functions functions;
	//check if current directory is workspace using filesystem class
    std::string path = std::filesystem::current_path().string();
	if (path.find("workspace") == std::string::npos)
	{
        readcfg();
	}

    size_t configSize = sizeof(configNames) / sizeof(configNames[0]);
    for (int i = 0; i < configSize; i++)
    {
        std::cout << "[" << i + 1 << "] " << configNames[i] << " config" << std::endl;
    }

    int input;
    functions.Log("Enter config number:", "Config Manager");
    std::cin >> input;

    createConfig(input, link);
}