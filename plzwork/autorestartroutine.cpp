#include "AutorestartClass.h"
#include <iostream>
#include <windows.h>
#include <Tlhelp32.h>
#include <chrono>
#include <fstream>
#include <tchar.h>
#include <thread>
#include <vector>
#include <filesystem>
#include <Lmcons.h>

#pragma warning(disable : 4996
// OpenSSL Library
#include <openssl/sha.h>

#ifdef _UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif // _UNICODE

typedef const TCHAR* LPCTSTR;

int RestartTime = 20;
void AutorestartClass::unlockRoblox()
{
	CreateMutex(NULL, TRUE, "ROBLOX_singletonMutex");
}

bool AutorestartClass::findRoblox()
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!Process32First(snapshot, &entry)) 
	{
		CloseHandle(snapshot);
		return false;
	}

	do {
		if (!_tcsicmp(entry.szExeFile, "RobloxPlayerBeta.exe")) 
		{
			CloseHandle(snapshot);
			return true;
		}
	} while (Process32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return false;
}

void AutorestartClass::killRoblox()
{
	system("cls");
	AutorestartClass Autorestart;
	Autorestart.Log("Killing Roblox", false);
	bool found = AutorestartClass::findRoblox() ? true : false;
	if (found) 
	{
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
		PROCESSENTRY32 pEntry;
		
		pEntry.dwSize = sizeof(pEntry);
		BOOL hRes = Process32First(hSnapShot, &pEntry);
		while (hRes) 
		{
			if (strcmp(pEntry.szExeFile, "RobloxPlayerBeta.exe") == 0) 
			{
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
				
				if (hProcess != NULL) 
				{
					TerminateProcess(hProcess, 9);
					CloseHandle(hProcess);
				}
				
			}
			hRes = Process32Next(hSnapShot, &pEntry);
		}
		CloseHandle(hSnapShot);
	}
}

bool AutorestartClass::inputToFile()
{
	std::string filename = "gettoken.py";
	std::ofstream outfile;
	outfile.open(filename);

	std::ifstream infile;
	infile.open("cookie.txt");

	std::string cookie;

	std::getline(infile, cookie);
	infile.close();

    	const std::string script = "import requests\n"
		"cookie = '" + cookie + "'\n"
		"sess = requests.session()\n"
		"sess.cookies['.ROBLOSECURITY'] = cookie\n"
		"csrf = sess.post('https://auth.roblox.com/v1/authentication-ticket').headers.get('x-csrf-token')\n"
		"sess.headers['x-csrf-token'] = csrf\n"
		"authticket = sess.post('https://auth.roblox.com/v1/authentication-ticket', headers={'Referer':'www.roblox.com','Origin':'www.roblox.com'}).headers.get('rbx-authentication-ticket')\n"
		"with open('tokens', 'w') as f:\n"
    		"    f.write(csrf +'\\n')\n"
    		"    f.write(authticket)";
	
	outfile << script;

	return true;
}


void AutorestartClass::Log(const std::string& text, bool error)
{
	std::cout << (error ? " [ error ] " + text : " [ Autorestart ] " + text + "                                       ") << std::endl;
}

void AutorestartClass::_usleep(int microseconds)
{
	std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

std::string AutorestartClass::SHA256(const char* path)
{
	std::ifstream fp(path, std::ios::in | std::ios::binary);
	
	if (!(fp.good())) 
	{
		std::ostringstream os;
		os << "cant open \"" << path << "\\";
		system("pause");
	}

	const std::size_t buffer_size { 1 << 12 };
	char buffer[buffer_size];

	unsigned char hash[SHA256_DIGEST_LENGTH] = { 0 };

	SHA256_CTX ctx;
	SHA256_Init(&ctx);

	while (fp.good()) 
	{
		fp.read(buffer, buffer_size);
		SHA256_Update(&ctx, buffer, fp.gcount());
	}

	SHA256_Final(hash, &ctx);
	fp.close();

	std::ostringstream os;
	os << std::hex << std::setfill('0');

	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) 
	{
		os << std::setw(2) << static_cast<unsigned int>(hash[i]);
	}

	return os.str();
}


void AutorestartClass::start()
{
	std::cout << "How many minutes before restarting? (default is 20): ";
	std::cin >> RestartTime;
	
	inputToFile();

	std::cout << "Are you using Synapse? (y/n)";
	char answer;
	std::cin >> answer;
	

	while (true)
	{
		AutorestartClass Autorestart;
		
		Autorestart.unlockRoblox();

		Autorestart.killRoblox();
		
		system("python gettoken.py");

		std::string csrf, authticket;
		std::ifstream myfile("tokens");
		if (myfile.is_open())
		{
			getline(myfile, csrf);
			getline(myfile, authticket);
			myfile.close();
		}
		else
		{
			std::cout << "Unable to open file";
			exit(0);
		}
		
	
		std::string path;
		if (answer == 'y')
		{
			//-- get absolute path of current directory
			path = std::filesystem::current_path().string();
			path.erase(path.find("workspace"), 9);

			path += "bin";

			//-- index every exe file in the bin folder and store them in a vector
			std::vector<std::string> files;
			for (const auto& entry : std::filesystem::directory_iterator(path)) 
			{
				if (entry.path().extension() == ".exe") 
				{
					files.push_back(entry.path().string());
				}
			}

			//-- hash every exe file in the bin folder and store them in a vector
			std::vector<std::string> hashes;
			for (const auto& file : files) 
			{
				hashes.push_back(SHA256(file.c_str()));
			}


			const std::string righthash = "40506136de9d0576fce7a09c9aab4b3076d29476056aac7540451e6daa7269b7";
			std::string rightpath;
			//-- compare the hashes against righthash and store the path of the right exe file in rightpath
			for (int i = 0; i < hashes.size(); i++) 
			{
				if (hashes[i] == righthash) 
				{
					rightpath = files[i];
				}
			}
			path = rightpath;
		}
		else
		{
			//-- get a vector of all folders present in C:\\Users
			std::vector<std::string> folders;
			for (auto& p : std::filesystem::directory_iterator("C:\\Users"))
			{
				folders.push_back(p.path().string());
			}

			//-- check which folders contains \AppData\Local\Roblox\, add its fuill path to a string
			std::string pathf;
			for (auto& p : folders)
			{
				if (std::filesystem::exists(p + "\\AppData\\Local\\Roblox\\"))
				{
					pathf = p + "\\AppData\\Local\\Roblox\\";
				}
			}

			//-- get all folders present in the path + \Versions except .exe files
			std::vector<std::string> Versions;
			for (auto& p : std::filesystem::directory_iterator(pathf + "\Versions"))
			{
				if (p.path().extension() != ".exe")
				{
					Versions.push_back(p.path().string());
				}
			}

			std::string path2 = Versions.back();

			std::string robloxplayerlauncher = '"' + path2 + "\\RobloxPlayerLauncher.exe" + '"';

			path = robloxplayerlauncher;
		}
		
		std::cout << path;
		system("pause");
		
		srand(time(NULL));

		std::string randomnumber = std::to_string(rand() % 100000 + 100000);
		std::string randomnumber2 = std::to_string(rand() % 100000 + 100000);
		std::string unixtime = std::to_string(std::time(nullptr));
		
		std::string browserTrackerID = randomnumber + randomnumber2;

		std::string command = '"' + path + '"' + " " + "roblox-player:1+launchmode:play+gameinfo:" + authticket + "+launchtime" + ':' + unixtime + "+placelauncherurl:" + "https%3A%2F%2Fassetgame.roblox.com%2Fgame%2FPlaceLauncher.ashx%3Frequest%3DRequestGame%26browserTrackerId%3D" + browserTrackerID + "%26placeId%3D" + "2809202155" + "%26isPlayTogetherGame%3Dfalse+" + "browsertrackerid:" + browserTrackerID + "+robloxLocale:en_us+gameLocale:en_us+channel:";
		
		//print command to console
		//std::cout << command << std::endl;
		//system("pause");
		
		system(command.c_str());
	
		
		auto start = std::chrono::steady_clock::now();
		
		while (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() <= RestartTime)
		{

			std::string msg = "(" + std::to_string(RestartTime - std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() + 1) + " minutes)";
			
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD coord = { 0, 0 };
			SetConsoleCursorPosition(hConsole, coord);

			Autorestart.Log(msg, false);

			Autorestart._usleep(10000);
			
		}
		Autorestart.killRoblox();
	}
}
