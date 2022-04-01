#include <fstream>

#include <ctime>
#include <chrono>

#include "log.hpp"


Log::~Log()
{
    Log::SaveToFile();
}

void Log::SaveToFile()
{
    Log& logManager = Log::Instance();

    // Create a new log file
    std::ofstream currentFile("build/FrakkasLog.log");

    // Put all the logs in it
    currentFile << logManager.logs;

    // Clear the logs
    logManager.logs.clear();
}

void Log::Out(const std::string& i_log)
{
    Instance().logList.push_back(i_log);

    // Get the current time as a char*
    std::time_t currentTime = std::time(nullptr);

    // TODO verify if ctime_r is working
    char timeString[26];
    //ctime_r(&currentTime, timeString);
    ctime_s(timeString, sizeof(timeString), &currentTime);
    timeString[24] = '\0';

    // Set the current log format
    std::string currentLog = "[" + std::string(timeString) + "] " + i_log + '\n';

    Log& logManager = Log::Instance();

    // Put the logs in the Log Manager
    logManager.logs += currentLog;
}