// Utils/Utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>

enum LogLevel {
    DEBUG,
    INFO,
    ERROR
};

class Utils {
public:
    static LogLevel currentLogLevel;

    static void logInfo(const std::string& message) {
        if(currentLogLevel <= INFO) {
            std::cout << "INFO: " << message << std::endl;
        }
    }

    static void logError(const std::string& message) {
        if(currentLogLevel <= ERROR) {
            std::cerr << "ERROR: " << message << std::endl;
        }
    }

    static void logDebug(const std::string& message) {
        if(currentLogLevel <= DEBUG) {
            std::cout << "DEBUG: " << message << std::endl;
        }
    }
};

#endif // UTILS_H
