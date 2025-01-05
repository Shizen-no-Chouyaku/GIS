// src/Config/ConfigManager.cpp

#include "ConfigManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

const std::string ConfigManager::CONFIG_FILE_PATH = "../config/settings.json";

AppConfig ConfigManager::loadConfig() {
    AppConfig cfg;
    // Provide some defaults
    cfg.fontPath = "../resources/fonts/WaukeganLdo-ax19.ttf";
    cfg.resolutionWidth = 1280;
    cfg.resolutionHeight = 720;

    std::ifstream file(CONFIG_FILE_PATH);
    if (!file.is_open()) {
        std::cerr << "[ConfigManager] Could not open config file: " 
                  << CONFIG_FILE_PATH << ". Using defaults.\n";
        return cfg;
    }

    try {
        nlohmann::json j;
        file >> j;
        file.close();

        if (j.contains("fontPath")) {
            cfg.fontPath = j.at("fontPath").get<std::string>();
        }
        if (j.contains("resolutionWidth")) {
            cfg.resolutionWidth = j.at("resolutionWidth").get<int>();
        }
        if (j.contains("resolutionHeight")) {
            cfg.resolutionHeight = j.at("resolutionHeight").get<int>();
        }
    } catch (std::exception& e) {
        std::cerr << "[ConfigManager] JSON parse error: " << e.what() 
                  << " - Using defaults.\n";
    }

    return cfg;
}

void ConfigManager::saveConfig(const AppConfig& config) {
    nlohmann::json j;
    j["fontPath"] = config.fontPath;
    j["resolutionWidth"] = config.resolutionWidth;
    j["resolutionHeight"] = config.resolutionHeight;

    std::ofstream file(CONFIG_FILE_PATH);
    if (!file.is_open()) {
        std::cerr << "[ConfigManager] Could not open config file for writing: " 
                  << CONFIG_FILE_PATH << "\n";
        return;
    }
    file << j.dump(4); // pretty-print with 4 spaces
    file.close();
    std::cout << "[ConfigManager] Config saved to " << CONFIG_FILE_PATH << "\n";
}
