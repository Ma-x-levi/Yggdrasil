#include <stdarg.h>

#include "NanoYggdrasil.h"


static DefaultYggdrasilOsalTaskManager   _defaultOsalTaskManager;
static DefaultYggdrasilOsalMutexManager  _defaultOsalMutexManager;


NanoYggdrasil& NanoYggdrasil::instance() 
{
    static NanoYggdrasil NanoYggdrasil;
    return NanoYggdrasil;
}


NanoYggdrasil::NanoYggdrasil() : 
_osalTaskManager(std::ref(_defaultOsalTaskManager)),
_osalMutexManager(std::ref(_defaultOsalMutexManager))
{
    
}




NanoYggdrasil::~NanoYggdrasil()
{

}


YggdrasilTypes::ReturnCode NanoYggdrasil::Init() noexcept
{
    if(std::addressof(_osalTaskManager.get()) == std::addressof(_defaultOsalTaskManager)) {
        LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::ERROR, "Yggdrasil osal task manager not registered.");
        return YggdrasilTypes::ReturnCode::NONSUPPORT;
    }

    if(std::addressof(_osalMutexManager.get()) == std::addressof(_defaultOsalMutexManager)) {
        LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::ERROR, "Yggdrasil osal mutex manager not registered.");
        return YggdrasilTypes::ReturnCode::NONSUPPORT;
    }



    LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::INFO, "Yggdrasil init started");
    LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::INFO, "Yggdrasil init successful");
    return YggdrasilTypes::ReturnCode::SUCCESS;
}


YggdrasilTypes::ReturnCode NanoYggdrasil::Execute() noexcept
{
    while(1){
        LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::INFO, "Yggdrasil execute");
        
        for(auto& handle : _halLampHandleList) {
            handle.get().Toggle();
        }

        _osalTaskManager.get().SleepMs(1000);
    }
}




