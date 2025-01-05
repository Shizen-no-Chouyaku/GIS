// src/Config/ConfigManager.h
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>

struct AppConfig {
    std::string fontPath;
    int resolutionWidth;
    int resolutionHeight;
};

class ConfigManager {
public:
    // Loads config from config/settings.json (if present),
    // or returns defaults if file not found or invalid.
    static AppConfig loadConfig();

    // Saves config to config/settings.json
    static void saveConfig(const AppConfig& config);

private:
    static const std::string CONFIG_FILE_PATH;
};

#endif // CONFIGMANAGER_H
