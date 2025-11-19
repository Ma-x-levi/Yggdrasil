#include "BackendService.h"

// #include <Yggdrasil.h>
// #include <YggdrasilLoggerBase.h>

#include <NanoYggdrasil.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>


int main(int argc, char *argv[]) 
{

    NanoYggdrasil::instance().Init();

    NanoYggdrasil::instance().Execute();

    // while(1){
    //     NanoYggdrasil::instance().ExtendLoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::ERROR,   "NanoYggdrasil error: %d",   1);
    //     NanoYggdrasil::instance().ExtendLoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::WARN,    "NanoYggdrasil warn: %d",    2);
    //     NanoYggdrasil::instance().ExtendLoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::INFO,    "NanoYggdrasil info: %d",    3);
    //     NanoYggdrasil::instance().ExtendLoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::DEBUG,   "NanoYggdrasil debug: %d",   4);
    //     NanoYggdrasil::instance().ExtendLoggerOutput(YggdrasilLoggerHandleBase::LoggerLevel::DEFAULT, "NanoYggdrasil default: %d", 5);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // }

    return EXIT_SUCCESS;
}
