#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

#include "request.h"
#include "configmanagerroutine.h"
#include "AutorestartClass.h"
#include "roblox.h"

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

int main(int argc, char* argv[])
{
	SetConsoleTitle("Floppafarm AIO tool Version 1.0 - by DarkNet#7679 & Sightem#4821 - https://discord.gg/u2vU98KfFS");

	
	bool ontop = 1;
	bool windowsize = 1;
	if (argc > 1)
	{
		for (int i = 0; i < argc; i++)
		{
			if (strcmp(argv[i], "notop") == 0) { ontop = 0;}
			
			if (strcmp(argv[i], "nolockwindowsize") == 0) { windowsize = 0;}
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

	//-- make a requst to get the newest version of the script
	Request req("https://api.sightem.dev");
	if (req.initalize() == 1) {
		std::cout << "Error: Could not initialize find newest version" << std::endl;
		return 1;
	}
	Response res = req.get();
	std::cout << "Welcome to Hooverfarm's AIO tool" << std::endl;

	//-- print options
	std::cout << "[1] Config manager" << std::endl;
	std::cout << "[2] Autorestart" << std::endl;
	
	//-- print Option choice:
	std::cout << "Option choice: ";
	
	//-- read the user's choice
	int choice;
	std::cin >> choice;
	
	//-- switch handling the user's choice
	switch (choice)
	{
	case 1:
		system("cls");
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
		Autorestart.start();
	}
}