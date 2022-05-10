
#include "log.h"
#include <cstdarg>
#include <cstdio>
#include <stdarg.h>

using namespace todd;

const char* LogLevelStr[] = {
    " ", "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

Logger::Logger()
    :output(stdout)
    , outputLogLv(LogLevel::DEBUG)
    , outputFlag(1)
{
}

Logger::~Logger()
{
    if (output != stdout) {
        std::fclose(output);
    }
}


void Logger::printLog(LogLevel lv, char const* format, ...)
{
    va_list ap;
    int count;

    va_start(ap, format);
    if (lv >= outputLogLv) {
        std::string result = printTime() + "[" + LogLevelStr[static_cast<int>(lv)] + "]: " + std::string(format);
        std::fprintf(getOutput(), result.c_str(), ap);
    }
    va_end(ap);
}


const std::string Logger::printTime()
{
    std::string time;
    return time;
}


void Logger::trace(const char *format, ...)
{
    std::va_list ap;
    va_start(ap, format);
    printLog(LogLevel::TRACE, format, ap);
    va_end(ap);
}


void Logger::info(char const* format, ...)
{
    std::va_list ap;
    va_start(ap, format);
    printLog(LogLevel::INFO, format, ap);
    va_end(ap);
}


void Logger::debug(char const* format, ...)
{
    std::va_list ap;
    va_start(ap, format);
    printLog(LogLevel::DEBUG, format, ap);
    va_end(ap);
}


void Logger::warning(char const* format, ...)
{
    std::va_list ap;
    va_start(ap, format);
    printLog(LogLevel::WARN, format, ap);
    va_end(ap);
}


void Logger::error(char const* format, ...)
{
    std::va_list ap;
    va_start(ap, format);
    printLog(LogLevel::WARN, format, ap);
    va_end(ap);
}


void Logger::fatal(char const* format, ...)
{
    std::va_list ap;
    va_start(ap, format);
    printLog(LogLevel::WARN, format, ap);
    va_end(ap);
}

void Logger::setLogLevel(LogLevel lv)
{
    outputLogLv = lv;
}


void Logger::setOutputFile(const std::string &fname)
{
    if (outputFlag != 1) {
        return;
    }
    std::FILE* outputfile = fopen(fname.c_str(), "a");
    output = outputfile;
}

std::FILE* Logger::getOutput()
{
    return output;
}


