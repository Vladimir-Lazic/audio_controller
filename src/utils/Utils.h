#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

class StringUtils {
public:
    static std::string trim(const std::string& s)
    {
        if (s.empty())
            return s;

        auto start = s.find_first_not_of(" \n\r\t");
        if (start == std::string::npos)
            return "";

        auto end = s.find_last_not_of(" \n\r\t");
        return s.substr(start, end - start + 1);
    }

    static std::vector<std::string> split(const std::string& str, const char delimiter)
    {
        std::vector<std::string> tokens;
        std::istringstream stream(str);
        std::string token;

        while (std::getline(stream, token, delimiter)) {
            tokens.emplace_back(trim(token));
        }

        return tokens;
    }
};

class TimeUtils {
public:
    static std::string getCurrentTimeStamp()
    {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);

        std::ostringstream os;
        os << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
        return os.str();
    }
};

#endif // TIME_UTILS_H
