#pragma once
#include <filesystem>
namespace fs = std::filesystem;

typedef bool (*validation_function)(const fs::directory_entry&);

struct search_context {
	validation_function validation;
	std::vector<fs::path> results;
	bool search_one;
};

int start_deep_traverse_search(fs::path, search_context*, int);
