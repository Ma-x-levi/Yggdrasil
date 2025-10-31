#pragma once

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>
#include "YggdrasilThreadPool.h"
#include "YggdrasilLoggerBase.h"

namespace yggdrasil::platform {

enum class YggdrasilState : std::uint8_t 
{
    Uninitialized = 0,
    Initializing,
    Initialized,
    Starting,
    Running,
    Stopping,
    Stopped,
    Error
};

struct YggdrasilConfig {
    std::string name{"Yggdrasil"};
    std::chrono::milliseconds gracefulShutdownTimeout{std::chrono::seconds(5)};
    bool trackMetrics{true};
    std::size_t workerCount{0};
};

struct YggdrasilStatus {
    YggdrasilState state{YggdrasilState::Uninitialized};
    bool initialized{false};
    bool running{false};
    std::string name;
    std::string message;
    std::chrono::steady_clock::time_point timestamp{};
    std::chrono::steady_clock::time_point startTime{};
    std::size_t tasksSubmitted{0};
    std::size_t tasksCompleted{0};
};

class Yggdrasil final 
{
public:

private:

    std::string       _name;

    std::atomic<bool> _initialized;
    std::atomic<bool> _root_running;
    std::mutex        _root_mutex;
    std::thread       _root_thread;


    YggdrasilThreadPool _threadPool;
    
public:



public:
    using StatusListener = std::function<void(const YggdrasilStatus&)>;
    using TaskId = YggdrasilThreadPool::TaskId;
    using TaskMetrics = YggdrasilThreadPool::TaskMetrics;

    static Yggdrasil& instance();

    [[nodiscard]] bool RegisterLoggerHandler(YggdrasilLoggerBase::Ptr logger);
    [[nodiscard]] YggdrasilLoggerBase::Ptr logger() const;

    bool initialize(const YggdrasilConfig& config = {});
    bool start();
    void stop();
    void shutdown();

    int main(int argc, char *argv[]);

    [[nodiscard]] TaskId submitTask(YggdrasilThreadBase::Ptr task);
    [[nodiscard]] TaskId startLoopTask(YggdrasilThreadBase::Ptr task);
    bool stopLoopTask(TaskId taskId);
    [[nodiscard]] std::optional<TaskMetrics> taskMetrics(TaskId taskId) const;
    [[nodiscard]] std::optional<TaskMetrics> loopTaskMetrics(TaskId taskId) const;

    bool isInitialized() const noexcept;
    bool isRunning() const noexcept;
    YggdrasilState state() const noexcept;
    YggdrasilStatus status() const;

    std::size_t addStatusListener(StatusListener listener);
    bool removeStatusListener(std::size_t token);

    void signalTaskSubmitted(std::size_t count = 1);
    void signalTaskCompleted(std::size_t count = 1);

private:
    Yggdrasil();

    void notifyStatusChange();
    YggdrasilStatus buildStatusUnsafe() const;
    void emitLog(YggdrasilLoggerBase::Level level, std::string_view message, std::string_view category = {}) const;

    mutable std::mutex _mutex;
    mutable std::mutex _loggerMutex;
    YggdrasilConfig _config;
    std::atomic<YggdrasilState> _state;
    std::atomic<bool> _running;
    std::chrono::steady_clock::time_point _startTime;
    std::string _lastMessage;
    std::atomic<std::size_t> _tasksSubmitted;
    std::atomic<std::size_t> _tasksCompleted;
    std::size_t _listenerToken;
    std::unordered_map<std::size_t, StatusListener> _listeners;
    YggdrasilLoggerBase::Ptr _logger;
};

} // namespace yggdrasil::platform
