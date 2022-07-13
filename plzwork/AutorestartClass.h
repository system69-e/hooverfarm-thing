#pragma once
#include <string>

class AutorestartClass
{
public:
	void start();
	void unlockRoblox();
	bool findRoblox();
	void killRoblox();
	bool inputToFile();
	void Log(const std::string& text, bool error);

	void _usleep(int microseconds);
};