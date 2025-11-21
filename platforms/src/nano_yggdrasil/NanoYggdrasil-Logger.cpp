#include <stdarg.h>

#include "NanoYggdrasil.h"


YggdrasilTypes::ReturnCode NanoYggdrasil::RegisterLoggerHandle(std::reference_wrapper<YggdrasilLoggerHandleBase> handle) noexcept
{
    _loggerHandleList.push_back(handle);
    return YggdrasilTypes::ReturnCode::SUCCESS;
}


YggdrasilTypes::ReturnCode NanoYggdrasil::LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel level, std::string_view string) noexcept
{
    char output[string.length() + YggdrasilTypes::LOGGER_DEFAULT_FORMAT_SIZE::value] = {0};
    float time = 0.0f;
    switch (level)
    {
        case YggdrasilLoggerHandleBase::LoggerLevel::ERROR:   snprintf(output, sizeof(output), "%s[%.3f]-[Error]-%s%s",   YggdrasilTypes::LOGGER_LEVEL_ERROR_COLOR.data(),   time, string.data(), YggdrasilTypes::LOGGER_LEVEL_DEFAULT_COLOR.data()); break;
        case YggdrasilLoggerHandleBase::LoggerLevel::WARN:    snprintf(output, sizeof(output), "%s[%.3f]-[Warn]-%s%s",    YggdrasilTypes::LOGGER_LEVEL_WARN_COLOR.data(),    time, string.data(), YggdrasilTypes::LOGGER_LEVEL_DEFAULT_COLOR.data()); break;
        case YggdrasilLoggerHandleBase::LoggerLevel::INFO:    snprintf(output, sizeof(output), "%s[%.3f]-[Info]-%s%s",    YggdrasilTypes::LOGGER_LEVEL_INFO_COLOR.data(),    time, string.data(), YggdrasilTypes::LOGGER_LEVEL_DEFAULT_COLOR.data()); break;
        case YggdrasilLoggerHandleBase::LoggerLevel::DEBUG:   snprintf(output, sizeof(output), "%s[%.3f]-[Debug]-%s%s",   YggdrasilTypes::LOGGER_LEVEL_DEBUG_COLOR.data(),   time, string.data(), YggdrasilTypes::LOGGER_LEVEL_DEFAULT_COLOR.data()); break;
        case YggdrasilLoggerHandleBase::LoggerLevel::DEFAULT: snprintf(output, sizeof(output), "%s[%.3f]-[Default]-%s%s", YggdrasilTypes::LOGGER_LEVEL_DEFAULT_COLOR.data(), time, string.data(), YggdrasilTypes::LOGGER_LEVEL_DEFAULT_COLOR.data()); break;
        default:                                              snprintf(output, sizeof(output), "%s[%.3f]-[Default]-%s%s", YggdrasilTypes::LOGGER_LEVEL_DEFAULT_COLOR.data(), time, string.data(), YggdrasilTypes::LOGGER_LEVEL_DEFAULT_COLOR.data()); break;
    }

    for(auto& loggerHandle : _loggerHandleList){
        loggerHandle.get().LoggerOutput(level, output);
    }
    return YggdrasilTypes::ReturnCode::SUCCESS;
}


YggdrasilTypes::ReturnCode NanoYggdrasil::ExtendLoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel level, std::string_view fmt, ...) noexcept
{
    va_list args;
    char string[fmt.length() + YggdrasilTypes::LOGGER_DEFAULT_PARAMETER_SIZE::value] = {0};

    va_start(args, fmt);
    vsnprintf(string, sizeof(string), fmt.data(), args);
    va_end(args);

    LoggerOutput(level, string);
    return YggdrasilTypes::ReturnCode::SUCCESS;
}


