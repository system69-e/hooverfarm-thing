#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <vector>
#include <sstream>
#include <tuple>
#include <filesystem>

//-- User libs
#include "terminal.h"
#include "ConfigManager.h"
#include "CustomConfig.h"
#include "logger.h"

namespace fs = std::filesystem;

const std::string configFile = "hooverYBA.lua";
std::string basePath = "../autoexec/";
std::string autoExecPath = basePath + configFile;
std::string customConfigsPath = "../aio/";

void readcfg()
{
    std::ifstream configFileStream("config.ini");
    if (!configFileStream.is_open())
    {
        Log("Could not open config file", LOG_ERROR);
        return;
    }
    std::string line;
    std::getline(configFileStream, line);

    //get the parent folder of line
	std::string parentFolder = line.substr(0, line.find_last_of("\\"));
	basePath = parentFolder + "/autoexec/";
    autoExecPath = basePath + configFile;
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

// for custom config
ConfigOption options[]{
    {.name = "Teleport Timing", .type = ConfigOption::FLOAT, .offset = offsetof(Config, Teleport_Timing), opt_default(f, 0.5f)},
    {.name = "Use Ribcages", .type = ConfigOption::BOOL, .offset = offsetof(Config, Use_Ribcages), opt_default(b, false)},
    {.name = "Farm Shinys", .type = ConfigOption::BOOL, .offset = offsetof(Config, Farm_Shinys), opt_default(b, true)},
    {.name = "Farm Pity", .type = ConfigOption::BOOL, .offset = offsetof(Config, Pity_Config.Farm_Pity), opt_default(b, false)},
    {.name = "Pity Goal", .type = ConfigOption::FLOAT, .offset = offsetof(Config, Pity_Config.Pity_Goal), .depends = offsetof(Config, Pity_Config.Farm_Pity)},
    {.name = "Reduce Crashes", .type = ConfigOption::BOOL, .offset = offsetof(Config, Reduce_Crashes), opt_default(b, true)},
    {.name = "Anti Lag", .type = ConfigOption::BOOL, .offset = offsetof(Config, Anti_Lag), opt_default(b, true)},
    //{.name = "Webhook URL",.type = ConfigOption::STRING,.offset = offsetof(Config, Webhook_config.Webhook_URL)},
    //{.name = "Use Webhook",.type = ConfigOption::BOOL,.offset = offsetof(Config, Webhook_config.Use_Webhook)},
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
    "Ancient Scroll",
    "Steel Balls",
	"Rokakaka",
};

#define option(name) "getgenv()[\"" << #name << "\"] = " << config.name << std::endl; 
#define bool_option(name) "getgenv()[\"" << #name << "\"] = " << (config.name ? "true" : "false") << std::endl;
#define named_option(name, value) "[\"" << #name << "\"] = " << config.value << std::endl;
#define named_string_option(name, value) "[\"" << #name << "\"] = " << '"' << config.value << '"'<< std::endl;
#define named_bool_option(name, value) "[\"" << #name << "\"] = " << (config.value ? "true" : "false") << ',' << std::endl;

bool read_pity(Config* cfg) 
{
    std::ignore = getc(stdin); //-- skip remainin chars
read:
    Log("Enter pity goal: ", "ConfigManager", false);

    float f;
    try
    {
        std::string buffer = "";
        char c;

        while ((c = getchar()) != '\n')
            buffer += c;

        if (std::stof(buffer) < 1.5f || std::stof(buffer) > 5.0f)
        {
            Log("Invalid input, please try again!", LOG_ERROR);
            goto read;
        }

        if (buffer.empty())
            f = 2.0f;
        else f = std::stof(buffer);
    }
    catch (...)
    {
        Log("Invalid input, please try again!", LOG_ERROR);
        goto read;
    }

    cfg->Pity_Config.Pity_Goal = f;
    return true;
}
/*
bool read_webhook(Config* cfg) {
    getc(stdin);
    std::cout << "Do you want to use the webhook feature? (y/n)";
    char c;
    std::cin >> c;
    switch (c)
    {
      case 'y': case 'Y':
          cfg->Webhook_config.Use_Webhook = true;
          break;
      case 'n': case 'N':
          config.Webhook_config.Use_Webhook = false;
          break;
      default:
            config.Webhook_config.Use_Webhook = false;
          break;
    }

    if (cfg->Webhook_config.Use_Webhook)
    {
      std::cout << "Enter webhook URL: ";
      std::string buffer;
      std::cin >> buffer;
      cfg->Webhook_config.Webhook_URL = buffer;
    }
}
*/

std::vector<int> read_items() {
    std::ignore = getc(stdin); //-- skip remaining chars
    Log("Items to farm:", "ConfigManager");
    for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++)
    {
        std::cout << "     " << i + 1 << "." << items[i] << std::endl;
    }
    Log("Enter the items you want to farm (example: 1,3,6): ", "ConfigManager", false);

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
    return user_choice;
}

bool Configmanager::inputToFile(const std::string& filename, Config config)
{
	//-- file creation
    std::ofstream file;
    file.open(filename);

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

read_items:
	std::vector<int> user_choice = read_items();

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
            Log("Invalid input, try again.", LOG_ERROR);
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

bool handle_config(Configmanager* manager, Config config = configurations[0]) 
{
    if (config.Pity_Config.Farm_Pity) 
    {
        read_pity(&config);
    }
    //if (config.Webhook_config.Use_Webhook) {
    //	read_webhook(&config);
    //}
    return manager->inputToFile(configFile, config);
}

void Configmanager::createConfig(int input)
{
    clear();
    if (handle_config(this, configurations[input - 1]))
    {
        Log("Config created successfully", "ConfigManager");
    }
    else
    {
        Log("Config creation failed", LOG_ERROR);
    }
    //-- move the file to the autoexec folder
    fs::rename(configFile, autoExecPath);
    clear();
}

void Configmanager::createCustomConfig() {
    clear();
    std::string filename;
	std::cout << "Enter the name of the file: ";
	std::cin >> filename;

    // check if file exists
	if (fs::exists(customConfigsPath + filename))
	{
        Log("File already exists. It will be overwritten. Press enter to proceed", LOG_WARNING);
        std::ignore = getc(stdin);
        wait();
		// delete old file
		fs::remove(customConfigsPath + filename);
	}
	
    Config config{};
    std::ignore = getc(stdin); //-- skip remaining chars
    parse_custom(options, sizeof(options) / sizeof(options[0]), &config);
	if (inputToFile(filename, config))
	{
		Log("Config created successfully", "ConfigManager");
	}
	else
	{
		Log("Config creation failed", LOG_ERROR);
	}
	
	//-- move the file to the autoexec folder
    fs::copy(filename, customConfigsPath + filename); // save to autoexec
	fs::rename(filename, autoExecPath); // but then move the file to the custom configs folder
    clear();
}

/*
bool Configmanager::checkConfig()
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
*/

void Configmanager::customConfigHandler(Configmanager* manager) 
{

    while (true)
    {
        clear();
        Log("Chose one of the options below:", "ConfigManager");
        std::cout << "[1] Choose one of your already created configurations" << std::endl;
        std::cout << "[2] Create a new configuration" << std::endl;
		std::cout << "[3] Go back to main menu" << std::endl; 

        if (!fs::exists(customConfigsPath))
        {
            fs::create_directory(customConfigsPath);
        }

        int input;
        std::cin >> input;

        if (input == 1) 
        {
            // read the configs from the folder and list the names, then handle reading the configurations and putting them into the the console
            std::vector<std::string> configs;
            for (auto& p : fs::directory_iterator(customConfigsPath))
            {
                configs.push_back(p.path().filename().string());
            }

            if (configs.empty())
            {
                Log("No configurations created", "ConfigManager");
                std::ignore = getc(stdin);
                wait();
                return;
            }

            Log("Avaiable configurations: ", "ConfigManager");
            for (int i = 0; i < configs.size(); i++) 
            {
                std::cout << '[' << i + 1 << "] " << configs[i] << std::endl;
            }
            Log("Select one option: ", "ConfigManager", false);
            std::cin >> input;
            if (input > 0 && input <= configs.size()) 
            {
                // copy file to autoexec and rename it to the name of the file
                fs::copy(customConfigsPath + configs[input - 1], basePath);
                fs::rename(basePath + configs[input - 1], autoExecPath);
            }
            else 
            {
                Log("Invalid input, try again.", LOG_ERROR);
            }
        } 
        else if (input == 3)
        {
            main(0, (char**)"balls");
        }
        else 
        {
            manager->createCustomConfig();
        }
    }

}

void Configmanager::configManager()
{
    while (true)
    {
        Log("Chose one of the options below:", "ConfigManager");
        std::cout << "[1] Choose one of the preconfigured options" << std::endl;
        std::cout << "[2] Manage custom created configurations" << std::endl;
        std::cout << "[3] Go back to main menu" << std::endl;
		
        int input;
        std::cin >> input;
        if (input == 1)
        {
            //check if current directory is workspace using filesystem class
            std::string path = std::filesystem::current_path().string();
            if (path.find("workspace") == std::string::npos)
            {
                readcfg();
            }

            size_t configSize = sizeof(configNames) / sizeof(configNames[0]);
            int i = 0;
            for (; i < configSize; i++)
            {
                std::cout << "[" << i + 1 << "] " << configNames[i] << " config" << std::endl;
            }
            std::cout << '[' << configSize + 1 << ']' << " Go back to main menu" << std::endl;

            int innerinput;
            Log("Enter config number:", "ConfigManager");
            std::cin >> innerinput;

            if (innerinput != 4)
            {
                createConfig(innerinput);
            }
            else
            {
                main(0, (char**)"balls");
            }
        } 
        else if (input == 2)
        {
            customConfigHandler(this);
        }
        else if (input == 3)
		{
            main(0, (char**)"balls");
		}
        else
        {
            Log("Invalid input, try again.", LOG_ERROR);
        }
    }
}