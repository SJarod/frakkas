#include <ctime>
#include <chrono>
#include <iomanip>

#include "log.hpp"


Log::~Log()
{
    Instance().logFile.close();
}

void Log::Out(const std::string& i_log)
{
    Instance().logList.emplace_back(i_log);

    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    Instance().logFile << std::put_time(std::localtime(&now), "[%d-%m-%Y %H:%M:%S] ") << i_log << std::endl;
}