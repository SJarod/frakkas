#pragma once

#include "singleton.hpp"

#include <fstream>
#include <vector>
#include <format>

#include "singleton.hpp"


enum class ELogType
{
    INFO,
    WARNING,
    ERROR,
};

class Log : public Singleton<Log>
{
    friend class Singleton<Log>;

public:
    Log() = default;
    ~Log();

    static void Init()
    {
        Instance().logFile.open("build/FrakkasLog.log", std::ios::out);
    }

    static void Info(const std::string& i_log)
    {
        Out("[INFO] " + i_log);
    }

    static void Warning(const std::string& i_log)
    {
        Out("[WARNING] " + i_log);
    }

    static void Error(const std::string& i_log)
    {
        Out("[ERROR] " + i_log);
    }

    static std::vector<std::string>& GetLogList()
    {
        return Instance().logList;
    }

private:
    std::ofstream logFile;
    std::vector<std::string> logList;

    /**
    * @Summary Output the casted log
    */
    static void Out(const std::string& i_log);
};

