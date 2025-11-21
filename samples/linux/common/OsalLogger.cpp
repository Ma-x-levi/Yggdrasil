#include "OsalLogger.h"


OsalLogger::OsalLogger(std::string_view name, bool supportColor) :
YggdrasilLoggerHandleBase(name, supportColor)
{

}



YggdrasilTypes::ReturnCode OsalLogger::LoggerOutput(LoggerLevel level, std::string_view string) noexcept
{
    printf("%s\n", string.data());
    return YggdrasilTypes::ReturnCode::SUCCESS;
}


