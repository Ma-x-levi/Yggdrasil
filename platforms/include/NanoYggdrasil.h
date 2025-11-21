#pragma once

//c header
#include <stdio.h>

//cpp header
#include <vector>

//platform header
#include <hal/YggdrasilHalLampHandleBase.h>

#include <osal/YggdrasilLoggerHandleBase.h>
#include <osal/YggdrasilOsalTaskManagerBase.h>
#include <osal/YggdrasilOsalMutexManagerBase.h>




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

    std::reference_wrapper<YggdrasilOsalTaskManagerBase>              _osalTaskManager;

    std::reference_wrapper<YggdrasilOsalMutexManagerBase>             _osalMutexManager;

    std::vector<std::reference_wrapper<YggdrasilLoggerHandleBase>>    _loggerHandleList;   //Todo: use no new realize
    std::vector<std::reference_wrapper<YggdrasilHalLampHandleBase>>   _halLampHandleList;  //Todo: use no new realize

//method space
public:

    NanoYggdrasil();

    ~NanoYggdrasil();

    static NanoYggdrasil& instance();

    YggdrasilTypes::ReturnCode Init() noexcept;

    YggdrasilTypes::ReturnCode Execute() noexcept;

    YggdrasilTypes::ReturnCode RegisterOsalTaskManager(std::reference_wrapper<YggdrasilOsalTaskManagerBase> manager) noexcept;

    YggdrasilTypes::ReturnCode RegisterLoggerHandle(std::reference_wrapper<YggdrasilLoggerHandleBase> handle) noexcept;

    YggdrasilTypes::ReturnCode RegisterHalLampHandle(std::reference_wrapper<YggdrasilHalLampHandleBase> handle) noexcept;

    YggdrasilTypes::ReturnCode LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel level, std::string_view string) noexcept;

    /**
     * @brief Print out the selected level log of the specified format by the registration method.
     * @note The registered method is printed according to the corresponding level. If the level of the console is lower
     * than the level at which the log needs to be printed, it will not be printed successfully.
     * @param fmt: pointer to the format string that needs print out.
     * @param ...: Variable parameters, consistent with the use of the system interface print out.
     */
    YggdrasilTypes::ReturnCode ExtendLoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel level, std::string_view fmt, ...) noexcept;
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
