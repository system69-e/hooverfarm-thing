#pragma once
#include <string>
#include <vector>
#include <Windows.h>

class Autorestart
{
public:
	void start(bool);
	void unlockRoblox();
	bool findRoblox();
	void killRoblox();
	bool validateCookie();
	std::string SHA256(const char* str);

	void _usleep(int microseconds);
	void _sleep(int miliseconds);
	
private:
	std::vector<PROCESS_INFORMATION> robloxProcesses;
};
