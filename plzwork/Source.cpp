#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <Lmcons.h>

#include "Request.h"
#include "ConfigManager.h"
#include "Autorestart.h"
#include "Roblox.h"
#include "Terminal.h"
#include "FolderSearch.h"
#include "Logger.h"

// link libraries
#ifdef _DEBUG
#pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment (lib,"curl/libcurl_a.lib")
#endif

#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")
#pragma comment (lib, "User32.lib")

bool val_func(const fs::directory_entry& entry);
void createcfg();
std::vector<std::string> get_drives();

int main(int argc, char* argv[])
{
	SetConsoleTitle("Floppafarm AIO tool Version 1.9.3 - by DarkNet#7679 & Sightem#4821 - https://discord.gg/u2vU98KfFS");

	bool ontop = 1;
	bool windowsize = 1;
	bool forceminimize = 0;
	if (argc > 1)
	{
		for (int i = 0; i < argc; i++)
		{
			if (strcmp(argv[i], "notop") == 0) { ontop = 0;}
			
			if (strcmp(argv[i], "nolockwindowsize") == 0) { windowsize = 0;}

			if (strcmp(argv[i], "minimize") == 0) { forceminimize = 1; }
		}
	}

	//-- always on top
	if (ontop)
	{
		::SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		::ShowWindow(GetConsoleWindow(), SW_NORMAL);
	}
	
	//-- window size lock
	if (windowsize)
	{
		SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	}


	//-- get current username 
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName(username, &username_len);
	std::string username_str = username;
	
	//-- check if current directory is workspace, else, go stupid

	std::string current_path = std::filesystem::current_path().string();

	if (!(std::filesystem::exists("config.ini")))
	{

		if (current_path.find("workspace") == std::string::npos)
		{
			Log("Not in workspace, initializing search for workspace");
			
			if (!std::filesystem::exists("config.ini"))
			{
				createcfg();
			}

			search_context* ctx = new search_context;
			ctx->validation = val_func;
			ctx->search_one = false;
			ctx->results = std::vector<fs::path>();
			
			try 
			{
				start_deep_traverse_search(std::filesystem::current_path(), ctx, 2);
			}
			catch (const std::exception e) 
			{
				Log("Fatal file system error occured: " + std::string(e.what()), LOG_FATAL);
			}
			if (ctx->results.size() == 0)
			{
				//-- get a list of all drives
				std::vector<std::string> drives = get_drives();
				if (drives.size() > 1)
				{
					Log("No workspace folder detected in C drive, initializing search in other drives");

					for (int i = 1; i < drives.size(); i++)
					{
						start_deep_traverse_search(drives[i], ctx, 2);
					}
				}
				else
				{
					Log("No workspace folder was located.", LOG_ERROR);
					wait();
					return 0;
				}
			}

			if (ctx->results.size() > 1)
			{
				Log("Multiple results found, please select one: ");

				int outloop = 0;
				for (int i = 0; i < ctx->results.size(); i++)
				{
					std::cout << i << ": " << ctx->results[i].string() << std::endl;
					outloop++;
				}
				Log(outloop + ": None of these results are correct", LOG_ERROR);

				Log("Select a workspace: ");
				int selection = 0;
				std::cin >> selection;

				if (selection == outloop)
				{
					Log("No workspace selected, please manually move the exe to your workspace folder");
					wait();
					return 0;
				}

				std::ofstream config;
				config.open("config.ini");
				config << ctx->results[selection].string();
				config.close();
			}
			else
			{

				std::ofstream config;
				config.open("config.ini");
				config << ctx->results[0].string();
				config.close();
			}

			delete ctx;
		}
		else
		{
			Log("No config file found, creating one");
			std::ofstream config;
			config.open("config.ini");
			config << current_path;
		}
	}
	else
	{
		//-- check if the config file is empty
		std::ifstream config;
		config.open("config.ini");
		std::string config_path;
		config >> config_path;
		config.close();
		if (config_path.empty())
		{
			Log("Config file is empty, please manually move the exe to your workspace folder", LOG_ERROR);
			wait();
			return 0;
		}
	}
	
	//-- make a requst to get the newest version of the script
	Request req("https://api.sightem.dev");
	if (req.initalize() == 1)
	{
		Log("Could not initialize find newest version", LOG_FATAL);
		return 1;
	}
	Response res = req.get();
	Log("Welcome to Hooverfarm's AIO tool", "AIO");

	//-- print options
	std::cout << "[1] Config manager" << std::endl;
	std::cout << "[2] Autorestart" << std::endl;
	std::cout << "Option choice: ";
	
	//-- read the user's choice
	int choice;
	std::cin >> choice;
	
	//-- switch handling the user's choice
	switch (choice)
	{
	case 1:
		clear();
		Configmanager configmanager;
		configmanager.configManager(res.data);
		break;
	case 2:
		//check if cookie.txt is present, if not make one
		std::ifstream cookieFile("cookies.txt");
		if (!cookieFile.good())
		{
			Log("Cookie file not found, creating one", "AIO");
			std::ofstream cookieFile("cookies.txt");
			cookieFile << "";
			cookieFile.close();
			return 1;
		}
		else
		{
			Log("Cookie file found", "AIO");
		}

		Autorestart autorestart;
		autorestart.start(forceminimize);
	}
}

bool val_func(const fs::directory_entry& entry)
{
	//-- check for a valid workspace folder
	if (entry.path().filename() == "workspace")
	{
		for (auto& file : fs::directory_iterator(entry.path().parent_path()))
		{
			if (file.is_directory())
			{
				if (file.path().filename() == "autoexec")
				{
					return true;
				}
			}
		}
	}
	return false;
}

void createcfg()
{
	//-- create config.ini
	std::ofstream config;
	config.open("config.ini");
	config << "";
	config.close();
}

std::vector<std::string> get_drives()
{
	std::vector<std::string> drives;
	char drive_buffer[100];
	DWORD result = GetLogicalDriveStringsA(sizeof(drive_buffer), drive_buffer);
	if (result != 0)
	{
		char* drive_letter = drive_buffer;
		while (*drive_letter)
		{
			drives.push_back(drive_letter);
			drive_letter += strlen(drive_letter) + 1;
		}
	}
	return drives;
}
