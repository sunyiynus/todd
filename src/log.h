#ifndef TODD_SRC_LOG_H
#define TODD_SRC_LOG_H

#include <cstdio>
#include <memory>
#include <string>

#define todd_TRACE(s, ...) todd::Logger::logger().printLog(todd::LogLevel::TRACE, (s), __VA_ARGS__)
#define todd_DEBUG(s, ...) todd::Logger::logger().printLog(todd::LogLevel::DEBUG, (s), __VA_ARGS__)
#define todd_INFO(s, ...) todd::Logger::logger().printLog(todd::LogLevel::INFO, (s), __VA_ARGS__)
#define todd_WARN(s, ...) todd::Logger::logger().printLog(todd::LogLevel::WARN, (s), __VA_ARGS__)
#define todd_ERROR(s, ...) todd::Logger::logger().printLog(todd::LogLevel::ERROR, (s), __VA_ARGS__)
#define todd_FATAL(s, ...) todd::Logger::logger().printLog(todd::LogLevel::FATAL, (s), __VA_ARGS__)

namespace todd {


enum class LogLevel{
    TRACE = 1, DEBUG, INFO, WARN, ERROR, FATAL
};


const char* LogLevelStr[] = {
    " ", "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

class Logger
{
public:
    Logger();
    Logger(Logger &&) = default;
    Logger(const Logger &) = default;
    Logger &operator=(Logger &&) = default;
    Logger &operator=(const Logger &) = default;
    ~Logger();

    void trace(char const* format, ...);
    void info(char const* format, ...);
    void debug(char const* format, ...);
    void warning(char const* format, ...);
    void error(char const* format, ...);
    void fatal(char const* format, ...);

    void printLog(LogLevel lv, char const* format, ...);
    const std::string printTime();
    void setLogLevel(LogLevel lv);
    void setOutputFile(const std::string& fname);

    static Logger& logger();
private:
    FILE* getOutput();
private:
    std::FILE *output;
    std::string logFile;
    int outputFlag;

    LogLevel outputLogLv;
};

Logger& Logger::logger()
{
    static Logger logger;
    return logger;
}


} // namespace todd

#endif // !TODD_SRC_LOG_H
