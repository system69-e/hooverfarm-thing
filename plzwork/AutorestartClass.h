#pragma once
#include <string>
#include <Windows.h>

class AutorestartClass
{
public:
	void start(bool);
	void unlockRoblox();
	bool findRoblox();
	void killRoblox();
	void Log(const std::string& text, bool error);
	std::string SHA256(const char* str);

	void _usleep(int microseconds);
	void _sleep(int miliseconds);

private:
	PROCESS_INFORMATION robloxProcess{};
};