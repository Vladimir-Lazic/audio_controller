#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

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
