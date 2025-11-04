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

namespace {
volatile std::sig_atomic_t g_shutdownSignal = 0;

// class DemoConsoleLogger final : public yggdrasil::platform::YggdrasilLoggerBase {
// public:
//     DemoConsoleLogger()
//         : _stream(std::cout) {}

// protected:
//     void handleRecord(const Record& record) override {
//         std::lock_guard<std::mutex> lock(_streamMutex);
//         _stream << formatRecord(record) << std::endl;
//     }

//     void handleFlush() override {
//         std::lock_guard<std::mutex> lock(_streamMutex);
//         _stream << std::flush;
//     }

// private:
//     std::ostream& _stream;
//     std::mutex _streamMutex;
// };

class DemoOneShotTask final : public YggdrasilThreadBase {
public:
    DemoOneShotTask(std::string message, std::chrono::milliseconds delay)
        : _message(std::move(message))
        , _delay(delay)
        , _cancelled(false) {}

    void execute() override {
        if (_cancelled.load(std::memory_order_acquire)) {
            return;
        }

        if (_delay.count() > 0) {
            std::this_thread::sleep_for(_delay);
        }

        if (!_cancelled.load(std::memory_order_acquire)) {
            auto logger = Yggdrasil::instance().logger();
            const std::string text = "[BackendService Demo] One-shot task: " + _message;
            if (logger) {
                logger->info(text, "BackendDemo");
            } else {
                std::cout << text << std::endl;
            }
        }
    }

    void cancel() noexcept override {
        _cancelled.store(true, std::memory_order_release);
    }

    bool isCancellationRequested() const noexcept override {
        return _cancelled.load(std::memory_order_acquire);
    }

    std::string_view name() const noexcept override {
        return "BackendDemoOneShot";
    }

private:
    std::string _message;
    std::chrono::milliseconds _delay;
    std::atomic<bool> _cancelled;
};

class DemoLoopTask final : public YggdrasilThreadBase {
public:
    DemoLoopTask()
        : _cancelled(false)
        , _iteration(0) {}

    void execute() override {
        if (_cancelled.load(std::memory_order_acquire)) {
            return;
        }

        const auto iteration = _iteration.fetch_add(1, std::memory_order_relaxed) + 1;
        auto logger = Yggdrasil::instance().logger();
        std::ostringstream msg;
        msg << "[BackendService Demo] Loop iteration " << iteration;
        if (logger) {
            logger->debug(msg.str(), "BackendDemo");
        } else {
            std::cout << msg.str() << std::endl;
        }

        constexpr auto pause = std::chrono::milliseconds(750);
        std::this_thread::sleep_for(pause);
    }

    void cancel() noexcept override {
        _cancelled.store(true, std::memory_order_release);
    }

    bool isCancellationRequested() const noexcept override {
        return _cancelled.load(std::memory_order_acquire);
    }

    std::string_view name() const noexcept override {
        return "BackendDemoLoop";
    }

private:
    std::atomic<bool> _cancelled;
    std::atomic<std::uint64_t> _iteration;
};

void handleSignal(int signal) {
    g_shutdownSignal = signal;
}
} // namespace

int main(int argc, char *argv[]) 
{

    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    auto& yggdrasil = Yggdrasil::instance();

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

    // logBootstrap(YggdrasilLoggerBase::Level::Info, "Bootstrap logger configured.");
    // YggdrasilConfig config;
    // config.name = "BackendServiceSample";

    // if (!yggdrasil.initialize(config)) {
    //     // logBootstrap(YggdrasilLoggerBase::Level::Error, "Failed to initialize Yggdrasil runtime.");
    //     return EXIT_FAILURE;
    // }

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

    // yggdrasil.start();

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
