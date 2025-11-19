#include <stdarg.h>

#include "NanoYggdrasil.h"


NanoYggdrasil& NanoYggdrasil::instance() 
{
    static NanoYggdrasil NanoYggdrasil;
    return NanoYggdrasil;
}


NanoYggdrasil::NanoYggdrasil()
{
    
}




NanoYggdrasil::~NanoYggdrasil()
{

}


YggdrasilTypes::ReturnCode NanoYggdrasil::Init()
{
    LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::INFO, "Yggdrasil init started");
    LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::INFO, "Yggdrasil init successful");
    return YggdrasilTypes::ReturnCode::SUCCESS;
}


YggdrasilTypes::ReturnCode NanoYggdrasil::Execute()
{
    while(1){

    }
}




