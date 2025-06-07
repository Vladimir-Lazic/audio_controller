#ifndef LOGGER_H
#define LOGGER_H

#include "TimeUtils.h"
#include <fstream>
#include <mutex>
#include <string>

class Logger {
public:
    using Lock = std::lock_guard<std::mutex>;
    using LogLevel = std::string;

    Logger(const std::string& filePath)
    {
        logFile.open(filePath, std::ios::app);
    }

    ~Logger()
    {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const LogLevel& level, const std::string& message)
    {
        Lock l(logMutex);
        logFile << "[" << TimeUtils::getCurrentTimeStamp() << "] "
                << "[" << level << "] " << message << std::endl;
    }

private:
    std::ofstream logFile;
    std::mutex logMutex;
};

#endif // LOGGER_H
