#pragma once

//c header
#include <stdio.h>

//cpp header
#include <vector>

//platform header
#include <hal/YggdrasilHalLampHandleBase.h>
#include <osal/YggdrasilLoggerHandleBase.h>



class NanoYggdrasil
{

//define space
public:

//define space
protected:

//define space
private:


//variable space
public:

//variable space
protected:

//variable space
private:

    // YggdrasilLoggerHandleBase::HandlePtr loggerHandle;

    std::vector<std::reference_wrapper<YggdrasilLoggerHandleBase>>    _loggerHandleList;   //Todo: use no new realize
    std::vector<std::reference_wrapper<YggdrasilHalLampHandleBase>>   _halLampHandleList;  //Todo: use no new realize

//method space
public:

    NanoYggdrasil();

    ~NanoYggdrasil();

    static NanoYggdrasil& instance();

    YggdrasilTypes::ReturnCode Init();

    YggdrasilTypes::ReturnCode Execute();

    YggdrasilTypes::ReturnCode RegisterLoggerHandle(std::reference_wrapper<YggdrasilLoggerHandleBase> handle);

    YggdrasilTypes::ReturnCode RegisterHalLampHandle(std::reference_wrapper<YggdrasilHalLampHandleBase> handle);

    YggdrasilTypes::ReturnCode LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel level, std::string_view string);

    /**
     * @brief Print out the selected level log of the specified format by the registration method.
     * @note The registered method is printed according to the corresponding level. If the level of the console is lower
     * than the level at which the log needs to be printed, it will not be printed successfully.
     * @param fmt: pointer to the format string that needs print out.
     * @param ...: Variable parameters, consistent with the use of the system interface print out.
     */
    YggdrasilTypes::ReturnCode ExtendLoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel level, std::string_view fmt, ...);
//method space
protected:

//method space
private:


//interface space
public:

//interface space
protected:

//interface space
private:

};
