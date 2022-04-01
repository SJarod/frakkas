#pragma once

#include "singleton.hpp"

#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

#include "singleton.hpp"


enum class ELogType
{
    INFO,
    WARNING,
    ERROR,
    CRASH
};

class Log : public Singleton<Log>
{
    friend class Singleton<Log>;

public:
    Log() = default;
    ~Log();

    template <typename T>
    static void Info(const T& i_log)
    {
        Out(std::string("[INFO] ") + i_log);
    }

    template <typename T>
    static void Warning(const T& i_log)
    {
        Out(std::string("[WARNING] ") + i_log);
    }

    template <typename T>
    static void Error(const T& i_log)
    {
        Out(std::string("[ERROR] ") + i_log);
    }

    template <typename T>
    static void Crash(const T& i_log)
    {
        Out(std::string("[CRASH] ") + i_log);
    }

    static std::vector<std::string>& GetLogList()
    {
        return Instance().logList;
    }

private:
    std::vector<std::string> logList;
    std::string logs;

    static void SaveToFile();

    /**
    * @Summary Output the casted log
    */
    static void Out(const std::string& i_log);
};

