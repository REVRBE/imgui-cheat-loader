#pragma once
#include <unordered_map>
#include <string>

class c_globals {
public:
	bool active = true;
	char user_name[255] = "username";
	char pass_word[255] = "pass";

	std::string user_rank;

	bool isLoggedIn = false;
};

inline c_globals globals;