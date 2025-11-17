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



YggdrasilTypes::ReturnCode NanoYggdrasil::RegisterLoggerHandle(std::reference_wrapper<YggdrasilLoggerHandleBase> handle)
{
    loggerHandleList.push_back(handle);
    return YggdrasilTypes::ReturnCode::SUCCESS;
}

YggdrasilTypes::ReturnCode NanoYggdrasil::init()
{
    for(auto& loggerHandle : loggerHandleList){
        loggerHandle.get().LoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::INFO, "Yggdrasil init\n");
    }
    return YggdrasilTypes::ReturnCode::SUCCESS;
}


