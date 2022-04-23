#pragma once

#include "utils/singleton.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <vector>


enum class ELogType
{
    INFO,
    WARNING,
    ERROR
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

    /**
     * Print log on console, and save in .log file with label [INFO].
     * @param i_logs The logs to print, it could be any object that works with 'ostream<<' operator.
     */
    template <typename... Args>
    static void Info(Args&&... i_logs);
    /**
     * Print log on console, and save in .log file with label [WARNING].
     * @param i_logs The logs to print, it could be any object that works with 'ostream<<' operator.
     */
    template <typename... Args>
    static void Warning(Args&&... i_logs);
    /**
     * Print log on console, and save in .log file with label [ERROR].
     * @param i_logs The logs to print, it could be any object that works with 'ostream<<' operator.
     */
    template <typename... Args>
    static void Error(Args&&... i_logs);

    static std::vector<std::string>& GetLogList()
    {
        return Instance().logList;
    }

private:
    std::ofstream logFile;
    std::vector<std::string> logList;

    /**
     * Centralize all the arguments log into one string stream.
     * @param o_stream The output stream to print.
     * @param i_firstArg The first variadic object.
     * @param i_args The others variadic objects
     */
    template<typename TArg, typename... TArgs>
    static void GetFormat(std::stringstream& o_stream, TArg i_firstArg, TArgs&&... i_args);
    /**
     * This function end the GetFormat template process.
     * @param i_stream The string stream with all the logs.
     */
    static void GetFormat(std::stringstream& i_stream) {};

    /**
    * @Summary Output the casted log
    */
    static void Out(const std::string& i_log);
};

template <typename... Args>
void Log::Info(Args&&... i_logs)
{
    std::stringstream stream;
    GetFormat(stream, i_logs...);
    Out("[INFO] " + stream.str());
}

template <typename... Args>
void Log::Warning(Args&&... i_logs)
{
    std::stringstream stream;
    GetFormat(stream, i_logs...);
    Out("[WARNING] " + stream.str());
}

template <typename... Args>
void Log::Error(Args&&... i_logs)
{
    std::stringstream stream;
    GetFormat(stream, i_logs...);
    Out("[ERROR] " + stream.str());
}

template<typename TArg, typename... TArgs>
void Log::GetFormat(std::stringstream& o_stream, TArg i_firstArg, TArgs&&... i_args)
{
    o_stream << i_firstArg;
    GetFormat(o_stream, i_args...);
}

