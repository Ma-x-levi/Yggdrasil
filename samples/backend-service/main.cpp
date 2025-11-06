#include "BackendService.h"

#include <Yggdrasil.h>
#include <YggdrasilLoggerBase.h>

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

    // std::signal(SIGINT, handleSignal);
    // std::signal(SIGTERM, handleSignal);

    auto& yggdrasil = Yggdrasil::instance();

    yggdrasil.initialize();

    // auto consoleLogger = std::make_shared<DemoConsoleLogger>();
    // consoleLogger->setLevel(YggdrasilLoggerBase::Level::Debug);
    // yggdrasil.RegisterLoggerHandler(consoleLogger);
    // auto logger = consoleLogger;

    // auto logBootstrap = [&](YggdrasilLoggerBase::Level level, std::string_view message) {
    //     if (logger) {
    //         logger->log(level, message, "Bootstrap");
    //     } else {
    //         auto& stream = level >= YggdrasilLoggerBase::Level::Error ? std::cerr : std::cout;
    //         stream << std::string(message) << std::endl;
    //     }
    // };

    // const bool started = yggdrasil.start();
    // if (!started) {
    //     // logBootstrap(YggdrasilLoggerBase::Level::Error, "Failed to start Yggdrasil runtime.");
    //     yggdrasil.shutdown();
    //     return EXIT_FAILURE;
    // }

    // auto oneShotTask = std::make_shared<DemoOneShotTask>(
    //     "Backend service warm-up complete",
    //     std::chrono::milliseconds(250)
    // );

    // if (yggdrasil.submitTask(oneShotTask) == YggdrasilThreadPool::kInvalidTaskId) {
    //     // logBootstrap(YggdrasilLoggerBase::Level::Error, "Failed to submit demo one-shot task.");
    // } else {
    //     // logBootstrap(YggdrasilLoggerBase::Level::Info, "Submitted demo one-shot task.");
    // }

    // auto loopTask = std::make_shared<DemoLoopTask>();
    // const auto loopTaskId = yggdrasil.startLoopTask(loopTask);
    // if (loopTaskId == YggdrasilThreadPool::kInvalidTaskId) {
    //     // logBootstrap(YggdrasilLoggerBase::Level::Error, "Failed to start demo loop task.");
    // } else {
    //     // logBootstrap(YggdrasilLoggerBase::Level::Info, "Loop task started.");
    // }

    // BackendService service;
    // if (!service.initialize()) {
    //     std::cerr << "[Bootstrap] Backend service initialization failed." << std::endl;
    //     runtime.shutdown();
    //     return EXIT_FAILURE;
    // }

    // if (!runtime.start()) {
    //     std::cerr << "[Bootstrap] Failed to start Yggdrasil runtime." << std::endl;
    //     runtime.shutdown();
    //     return EXIT_FAILURE;
    // }

    // if (!service.start()) {
    //     std::cerr << "[Bootstrap] Backend service failed to start." << std::endl;
    //     runtime.stop();
    //     runtime.shutdown();
    //     return EXIT_FAILURE;
    // }

    // std::cout << "[Bootstrap] Backend service running. Press Ctrl+C to exit." << std::endl;

    // while (g_shutdownSignal == 0) {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // }

    // std::cout << "\n[Bootstrap] Shutdown requested (signal " << g_shutdownSignal << ")." << std::endl;

    // service.stop();
    // service.join();

    yggdrasil.start();

    

    while(1){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // yggdrasil.stop();
    // yggdrasil.shutdown();

    // std::uint32_t idleIterations = 0;
    // while (g_shutdownSignal == 0 && idleIterations < 20) {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(250));
    //     ++idleIterations;

    //     if (yggdrasil.submitTask(oneShotTask) == YggdrasilThreadPool::kInvalidTaskId) {
    //         // logBootstrap(YggdrasilLoggerBase::Level::Warn, "Unable to queue additional demo one-shot task.");
    //     }
    // }

    // if (g_shutdownSignal != 0) {
    //     std::ostringstream msg;
    //     msg << "Shutdown requested (signal " << g_shutdownSignal << ").";
    //     // logBootstrap(YggdrasilLoggerBase::Level::Info, msg.str());
    // }

    // if (loopTaskId != YggdrasilThreadPool::kInvalidTaskId) {
    //     yggdrasil.stopLoopTask(loopTaskId);
    //     // logBootstrap(YggdrasilLoggerBase::Level::Info, "Loop task stopped.");
    // }

    // yggdrasil.shutdown();

    // logBootstrap(YggdrasilLoggerBase::Level::Info, "Shutdown complete.");

    // if (logger) {
    //     logger->flush();
    // }
    return EXIT_SUCCESS;
}
