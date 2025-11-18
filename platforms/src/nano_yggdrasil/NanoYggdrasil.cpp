#include <stdarg.h>

#include "NanoYggdrasil.h"


NanoYggdrasil& NanoYggdrasil::instance() 
{
    static NanoYggdrasil NanoYggdrasil;
    return NanoYggdrasil;
}


NanoYggdrasil::NanoYggdrasil()
{
//     // printf("NanoYggdrasil created");
}




NanoYggdrasil::~NanoYggdrasil()
{

}


YggdrasilTypes::ReturnCode NanoYggdrasil::init()
{
    for(auto& loggerHandle : loggerHandleList){
        loggerHandle.get().LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::INFO, "Yggdrasil init\n");
    }
    return YggdrasilTypes::ReturnCode::SUCCESS;
}




