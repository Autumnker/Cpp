#include "logger.hpp"
#include <iostream>
#include <chrono>

//===== 内部函数 =====

using SysClock = std::chrono::system_clock;
using TimePoint = std::chrono::time_point<SysClock>;
std::time_t get_timestamp_()
{
    return SysClock::to_time_t(SysClock::now());
}

void Logger::info(const char* file, const char* function, int line, const char* msg)
{
    std::cout << "[" << get_timestamp_() << "] " << "[INFO]" \
        << file << " : " << function << " : " << line << " : " \
        << msg << std::endl;
}

void Logger::warn(const char* file, const char* function, int line, const char* msg)
{
    std::cout << "[" << get_timestamp_() << "] " << "[WARN]" \
        << file << " : " << function << " : " << line << " : " \
        << msg << std::endl;
}

void Logger::fail(const char* file, const char* function, int line, const char* msg)
{
    std::cout << "[" << get_timestamp_() << "] " << "[FAIL]" \
        << file << " : " << function << " : " << line << " : " \
        << msg << std::endl;
}
