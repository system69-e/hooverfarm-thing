#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <Lmcons.h>


#include "request.h"
#include "configmanagerroutine.h"
#include "AutorestartClass.h"
#include "roblox.h"
#include "terminal.h"
#include "folder_search.h"
#include "Functions.h"

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
	SetConsoleTitle("Floppafarm AIO tool Version 1.0 - by DarkNet#7679 & Sightem#4821 - https://discord.gg/u2vU98KfFS");

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
			std::cout << "Not in workspace, initializing search for workspace" << std::endl;
			
			if (!std::filesystem::exists("config.ini"))
			{
				createcfg();
			}

			search_context* ctx = new search_context;
			ctx->validation = val_func;
			ctx->search_one = false;
			ctx->results = std::vector<fs::path>();
			
			try {
				start_deep_traverse_search(std::filesystem::current_path(), ctx, 2);
			}
			catch (const std::exception e) {
				std::cout << "Fatal file system error occured: " << e.what() << std::endl;
			}
			if (ctx->results.size() == 0)
			{
				//-- get a list of all drives
				std::vector<std::string> drives = get_drives();
				if (drives.size() > 1)
				{
					std::cout << "No workspace folder detected in C, initializing search in other drives" << std::endl;

					for (int i = 1; i < drives.size(); i++)
					{
						start_deep_traverse_search(drives[i], ctx, 2);
					}
				}
				else
				{
					std::cout << "No workspace folder was located.";
					wait();
					return 0;
				}
			}

			if (ctx->results.size() > 1)
			{
				std::cout << "Multiple results found, please select one: " << std::endl;

				int outloop = 0;
				for (int i = 0; i < ctx->results.size(); i++)
				{
					std::cout << i << ": " << ctx->results[i].string() << std::endl;
					outloop++;
				}
				std::cout << outloop << ": None of these results are correct" << std::endl;

				std::cout << "Select a workspace: " << std::endl;
				int selection = 0;
				std::cin >> selection;

				if (selection == outloop)
				{
					std::cout << "No workspace selected, please manually move the exe to your workspace folder" << std::endl;
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
			std::cout << "Config file is empty, please manually move the exe to your workspace folder" << std::endl;
			wait();
			return 0;
		}
	}
	Functions functions;
	
	//-- make a requst to get the newest version of the script
	Request req("https://api.sightem.dev");
	if (req.initalize() == 1)
	{
		functions.Log("Could not initialize find newest version", "Error");
		return 1;
	}
	Response res = req.get();
	std::cout << "Welcome to Hooverfarm's AIO tool" << std::endl;

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
		ConfigmanagerClass Configmanager;
		Configmanager.configManager(res.data);
		break;
	case 2:
		//check if cookie.txt is present, if not make one
		std::ifstream cookieFile("cookies.txt");
		if (!cookieFile.good())
		{
			std::cout << "Cookie file not found, creating one" << std::endl;
			std::ofstream cookieFile("cookies.txt");
			cookieFile << "";
			cookieFile.close();
			return 1;
		}
		else
		{
			std::cout << "Cookie file found" << std::endl;
		}

		AutorestartClass Autorestart;
		Autorestart.start(forceminimize);
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
