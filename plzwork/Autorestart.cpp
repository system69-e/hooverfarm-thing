#include "Autorestart.h"
#include "Roblox.h"
#include "Terminal.h"
#include "Logger.h"
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

#pragma warning(disable : 4996)
// OpenSSL Library
#include <openssl/sha.h>


int RestartTime = 20;
void Autorestart::unlockRoblox()
{
	CreateMutex(NULL, TRUE, "ROBLOX_singletonMutex");
}

bool Autorestart::findRoblox()
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!Process32First(snapshot, &entry))
	{
		CloseHandle(snapshot);
		return false;
	}

	do 
	{
		if (!_tcsicmp(entry.szExeFile, "RobloxPlayerBeta.exe"))
		{
			CloseHandle(snapshot);
			return true;
		}
	} while (Process32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return false;
}

void Autorestart::killRoblox()
{
	system("cls");
	Log("Killing Roblox", "AutoRestart", true);
	bool found = Autorestart::findRoblox() ? true : false;
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


void Autorestart::_usleep(int microseconds)
{
	std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}
void Autorestart::_sleep(int miliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
}

std::string Autorestart::SHA256(const char *path)
{
	std::ifstream fp(path, std::ios::in | std::ios::binary);

	if (!(fp.good()))
	{
		std::ostringstream os;
		os << "cant open \"" << path << "\\";
		wait();
	}

	const std::size_t buffer_size{1 << 12};
	char buffer[buffer_size];

	unsigned char hash[SHA256_DIGEST_LENGTH] = {0};

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

void Autorestart::start(bool forceminimize)
{
	Log("How many minutes before restarting? ", "AutoRestart");
	std::cin >> RestartTime;

	Log("Are you using Synapse? (y/n)", "AutoRestart");
	char answer;
	std::cin >> answer;

	clear();

	std::ifstream infile;
	infile.open("cookies.txt");

	// parse each line into a string vector
	std::vector<std::string> cookies;
	std::string line;
	while (std::getline(infile, line))
	{
		cookies.push_back(line);
	}

	while (true)
	{
		//-- read the first line of config.ini, store into a string called line, stoer the second line into placeid
		std::ifstream config("config.ini");
		std::string line;
		std::string placeid = "2809202155";
		std::getline(config, line);
		std::getline(config, placeid);
		
		
		// for loop to iterate through each cookie
		for (int i = 0; i < cookies.size(); i++)
		{
			unlockRoblox();

			std::string authticket = getRobloxTicket(cookies.at(i));

			std::string path;
			if (answer == 'y')
			{

				// check if the current directory is called workspace
				std::string current_path = std::filesystem::current_path().string();


				if (current_path.find("workspace") == std::string::npos)
				{
					
					//-- extract the parent folder of line by removing \workspace
					std::string parent = line.substr(0, line.find_last_of("\\"));

					//-- iterate through the parent folder and find the bin folder
					std::filesystem::directory_iterator end_iter;
					for (std::filesystem::directory_iterator dir_iter(parent); dir_iter != end_iter; ++dir_iter)
					{
						if (std::filesystem::is_directory(dir_iter->status()))
						{
							//-- if the folder is bin, set the path to the bin folder
							if (dir_iter->path().filename() == "bin")
							{
								path = dir_iter->path().string();
							}
						}
					}
				}
				else
				{
					//-- get absolute path of current directory
					path = std::filesystem::current_path().string();
					path.erase(path.find("workspace"), 9);

					path += "bin";

					//-- index every exe file in the bin folder and store them in a vector
					std::vector<std::string> files;
					for (const auto &entry : std::filesystem::directory_iterator(path))
					{
						if (entry.path().extension() == ".exe")
						{
							files.push_back(entry.path().string());
						}
					}
				}

				//-- index every exe file in the bin folder and store them in a vector
				std::vector<std::string> files;
				for (const auto &entry : std::filesystem::directory_iterator(path))
				{
					if (entry.path().extension() == ".exe")
					{
						files.push_back(entry.path().string());
					}
				}

				//-- hash every exe file in the bin folder and store them in a vector
				std::vector<std::string> hashes;
				for (const auto &file : files)
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
				for (auto &p : std::filesystem::directory_iterator("C:\\Users"))
				{
					folders.push_back(p.path().string());
				}

				//-- check which folders contains \AppData\Local\Roblox\, add its fuill path to a string
				std::string pathf;
				for (auto &p : folders)
				{
					if (std::filesystem::exists(p + "\\AppData\\Local\\Roblox\\"))
					{
						pathf = p + "\\AppData\\Local\\Roblox\\";
					}
				}

				//-- get all folders present in the path + \Versions except .exe files
				std::vector<std::string> Versions;
				for (auto &p : std::filesystem::directory_iterator(pathf + "Versions"))
				{
					if (p.path().extension() != ".exe")
					{
						Versions.push_back(p.path().string());
					}
				}
				path = Versions.back() + "\\RobloxPlayerLauncher.exe";
			}

			srand((unsigned int)time(NULL));

			std::string randomnumber = std::to_string(rand() % 100000 + 100000);
			std::string randomnumber2 = std::to_string(rand() % 100000 + 100000);
			std::string unixtime = std::to_string(std::time(nullptr));
			std::string browserTrackerID = randomnumber + randomnumber2;

			std::string cmd = '"' + path + "\" " + "roblox-player:1+launchmode:play+gameinfo:" + authticket + "+launchtime" + ':' + unixtime + "+placelauncherurl:" + "https%3A%2F%2Fassetgame.roblox.com%2Fgame%2FPlaceLauncher.ashx%3Frequest%3DRequestGame%26browserTrackerId%3D" + browserTrackerID + "%26placeId%3D" + placeid + "%26isPlayTogetherGame%3Dfalse+" + "browsertrackerid:" + browserTrackerID + "+robloxLocale:en_us+gameLocale:en_us+channel:";

			STARTUPINFOA si = {};
			si.cb = sizeof(si);
			PROCESS_INFORMATION pi = {};
			if (!CreateProcess(&path[0], &cmd[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			{
				Log("CreateProcess() failed: " + GetLastError(), LOG_FATAL);
				return;
			}
			WaitForSingleObject(pi.hProcess, INFINITE);
			this->robloxProcesses.push_back(pi);
		}

		auto start = std::chrono::steady_clock::now();

		while (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() <= RestartTime)
		{
			if (forceminimize && FindWindow(NULL, "Roblox"))
			{
				for (int i = 0; i < cookies.size(); i++)
				{
					ShowWindow(FindWindow(NULL, "Roblox"), SW_FORCEMINIMIZE);
				}
			}

			std::string msg = "(" + std::to_string(RestartTime - std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() + 1) + " minutes)";

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD coord = {0, 0};
			SetConsoleCursorPosition(hConsole, coord);

			if (FindWindow(NULL, "ROBLOX Crash") || FindWindow(NULL, "Roblox Crash")) { goto error; };

			Log(msg, "AutoRestart");

			_usleep(10000);
		}
		error:

		killRoblox();
		_sleep(5000);
	}
}
