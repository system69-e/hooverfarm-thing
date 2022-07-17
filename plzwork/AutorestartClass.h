#pragma once
#include <string>

class AutorestartClass
{
public:
	void start();
	void unlockRoblox();
	bool findRoblox();
	void killRoblox();
	void Log(const std::string& text, bool error);
	std::string SHA256(const char* str);

	void _usleep(int microseconds);
};