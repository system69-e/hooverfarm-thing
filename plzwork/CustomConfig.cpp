#include <string>
#include <iostream>

//-- User libs
#include "configmanager.h"
#include "CustomConfig.h"

using std::string, std::cout;

#define write(T) *(T *)((char *)config + option.offset)
#define opt(T, off) *(T *)((char *)config + (off))

bool parse(Config* config, ConfigOption option)
{

    if (option.depends != nodepend)
    {
        if (!opt(bool, option.depends))
            return true;
    }

parse:
    cout << "Enter " << option.name << ": ";
    switch (option.type)
    {
    case ConfigOption::BOOL:
        cout << "(y/n) ";
        if (option.has_default)
            cout << "(default: " << (option.b ? "y" : "n") << ")";
        break;
    case ConfigOption::INT:
    case ConfigOption::FLOAT:
        cout << "(number) ";
        if (option.has_default)
            cout << "(default: " << (option.type == ConfigOption::FLOAT ? option.f : option.i) << ")";
        break;
    case ConfigOption::STRING:
        cout << "(name) ";
        if (option.has_default)
            cout << "(default: \"" << option.s << "\")";
        break;
    }

    // parse the option
    string buffer;
    char c;
    while ((c = getc(stdin)) != '\n')
        buffer += c;
    switch (option.type)
    {
    case ConfigOption::BOOL:
        if (buffer.empty())
            if (option.has_default)
                write(bool) = option.b;
            else
                goto parse;
        else if (buffer == "y" || buffer == "Y")
            write(bool) = true;
        else
            write(bool) = false;
        break;
    case ConfigOption::INT:
        if (buffer.empty())
            if (option.has_default)
                write(int) = option.i;
            else
                goto parse;
        else
            write(int) = atoi(buffer.c_str());
        break;
    case ConfigOption::FLOAT:
        if (buffer.empty())
            if (option.has_default)
                write(float) = option.f;
            else
                goto parse;
        else
            write(float) = (float) atof(buffer.c_str());
        break;
    case ConfigOption::STRING:
        if (buffer.empty())
            if (option.has_default)
                write(string) = string(option.s);
            else
                goto parse;
        else
            write(string) = string(buffer.c_str());
        break;
    }
    return true;
}

bool parse_custom(ConfigOption options[], size_t size, Config* config) {
	for (size_t i = 0; i < size; i++) {
		if (!parse(config, options[i]))
			return false;
	}
	return true;
}