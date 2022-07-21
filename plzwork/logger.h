#pragma once
#include <string>

enum LogLevel
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL
};

static std::string LogLevelNames[] =
{
	"DEBUG",
	"INFO",
	"WARNING",
	"ERROR",
	"FATAL"
};

// static log functions
void Log(const std::string& text, LogLevel level = LOG_INFO, bool endl = 1);

void Log(const std::string&, std::string, bool endl = 1);