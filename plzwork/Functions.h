#pragma once
#include <string>

//trolling
constexpr std::size_t operator""_str(const char* str, std::size_t size)
{
	std::size_t digest = 0;
	while (size-- > 0)
	{
		char val = *str;
		digest ^= val * val ^ val;
		++str;
	}
	return digest;
}

using std_string = decltype(std::string{ "kek" });
struct string : std_string
{
	using std_string::std_string;
	operator int() noexcept(noexcept(std_string::c_str()))
	{
		return operator""_str(std_string::c_str(), std_string::size());
	}
};

class Functions
{
public:
	void Log(const std::string&, string);
};