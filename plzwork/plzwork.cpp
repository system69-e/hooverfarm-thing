#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <fstream>

#include "curl/curl.h"
#include "configmanagerroutine.h"
#include "AutorestartClass.h"

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

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main()
{
	SetConsoleTitle("Floppafarm AIO tool Version 1.0 - by DarkNet#7679 & Sightem#4821 - https://discord.gg/u2vU98KfFS");
	
    //-- retrieve the contents of https://api.sightem.dev
	std::string linkBuffer;

	//-- curl stuff
	CURL* curl;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.sightem.dev");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &linkBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if (res != CURLE_OK)
		{
			std::cout << "Failed to grab the newest version" << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "Failed to grab the newest version" << std::endl;
		return 1;
	}


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
		Configmanager.configManager(linkBuffer);
		break;
	case 2:
		//check if cookie.txt is present, if not make one
		std::ifstream cookieFile("cookie.txt");
		if (!cookieFile.good())
		{
			std::cout << "Cookie file not found, creating one" << std::endl;
			std::ofstream cookieFile("cookie.txt");
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