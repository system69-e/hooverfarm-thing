#pragma once
#include <string>

struct Config
{
    float Teleport_Timing; // offset 0
    bool Use_Ribcages; // offset sizeof(float)
    bool Farm_Shinys; // offset sizeof(float) + sizeof(bool)
    struct {
        bool Farm_Pity;
        float Pity_Goal;
    } Pity_Config;
    bool Reduce_Crashes;
    bool Anti_Lag;
    /*
    struct {
    bool Use_Webhook;
    std::string Webhook_URL;
    } Webhook_config;
  */
};

class Configmanager
{
public:
    Configmanager(std::string link) : link(link) {}

	void createConfig(int input);
    void createCustomConfig();
	void configManager();
	bool checkConfig();
    bool inputToFile(const std::string& filename, Config config);
protected:
    std::string link;
};