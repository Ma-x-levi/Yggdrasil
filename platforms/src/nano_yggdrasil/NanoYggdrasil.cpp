#include <stdarg.h>

#include "NanoYggdrasil.h"

NanoYggdrasil::NanoYggdrasil()
{
//     // printf("NanoYggdrasil created");
}




NanoYggdrasil::~NanoYggdrasil()
{

}

void NanoYggdrasil::test()
{
    printf("nano yggdrasil test\n");
}


YggdrasilTypes::ReturnCode NanoYggdrasil::init()
{
    for(auto& loggerHandle : loggerHandleList){
        loggerHandle.get().LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::INFO, "Yggdrasil init\n");
    }
    return YggdrasilTypes::ReturnCode::SUCCESS;
}




