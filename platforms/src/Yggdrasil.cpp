#include "Yggdrasil.h"
#include <thread>
#include <algorithm>
#include <utility>

namespace yggdrasil::platform {

namespace {
constexpr const char* kAlreadyInitializedMsg = "Yggdrasil is already initialized.";
constexpr const char* kInitializeMsg = "Yggdrasil initialized.";
constexpr const char* kStartBeforeInitMsg = "Start requested before initialization.";
constexpr const char* kStartMsg = "Yggdrasil started.";
constexpr const char* kStopMsg = "Yggdrasil stopped.";
constexpr const char* kStoppingMsg = "Yggdrasil stopping.";
constexpr const char* kShutdownMsg = "Yggdrasil shutdown complete.";
} // namespace

Yggdrasil& Yggdrasil::instance() {
    static Yggdrasil g_instance;
    return g_instance;
}

Yggdrasil::Yggdrasil()
    : _config{}
    , _state(YggdrasilState::Uninitialized)
    , _initialized(false)
    , _root_running(false)
    , _running(false)
    , _startTime{}
    , _lastMessage("Not initialized.")
    , _tasksSubmitted(0)
    , _tasksCompleted(0)
    , _listenerToken(0) {}

bool Yggdrasil::initialize(const YggdrasilConfig& config) {
    printf("run initialized\n");
    {
        std::scoped_lock lock(_mutex);
        if (_initialized.load(std::memory_order_acquire)) {
            _config = config;
            _lastMessage = kAlreadyInitializedMsg;
            return true;
        }

        _config = config;
        _state.store(YggdrasilState::Initialized, std::memory_order_release);
        _initialized.store(true, std::memory_order_release);
        _lastMessage = kInitializeMsg;
    }
    notifyStatusChange();
    return true;
}


bool Yggdrasil::RegisterLoggerHandler(YggdrasilLoggerBase::Ptr logger)
{
    return false;
}



int Yggdrasil::main(int argc, char *argv[])
{
    std::unique_lock<std::mutex> lock(_root_mutex, std::defer_lock);
    if(!lock.try_lock()) {
        //Todo: do something return , printf or logger
        return -1;
    }

    // initialize the root thread
    _root_running.store(true, std::memory_order_release);
    while(_root_running.load(std::memory_order_acquire)) {
        printf("run\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

bool Yggdrasil::start()
{
    // {
    //     std::scoped_lock lock(_mutex);。
    //     if (!_initialized.load(std::memory_order_acquire)) {
    //         _state.store(YggdrasilState::Error, std::memory_order_release);
    //         _lastMessage = kStartBeforeInitMsg;
    //         return false;
    //     }

    //     if (_running.load(std::memory_order_acquire)) {
    //         return true;
    //     }

    //     _state.store(YggdrasilState::Running, std::memory_order_release);
    //     _running.store(true, std::memory_order_release);
    //     _startTime = std::chrono::steady_clock::now();
    //     _lastMessage = kStartMsg;
    // }
    // notifyStatusChange();

    // bool expected = false;
    // if (_root_running.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
    //     if (_root_thread.joinable()) {
    //         _root_thread.join();
    //     }

    //     _root_thread = std::thread([this]() {
    //         std::unique_lock<std::mutex> lock(_root_mutex, std::defer_lock);
    //         if (!lock.try_lock()) {
    //             _root_running.store(false, std::memory_order_release);
    //             return;
    //         }

    //         while (_root_running.load(std::memory_order_acquire)) {
    //             printf("run\n");
    //             std::this_thread::sleep_for(std::chrono::seconds(1));
    //         }
    //     });
    // }


    if(_root_thread.joinable()){
        _root_thread.join();
    }

    _root_thread = std::thread(&Yggdrasil::main, this, 0, nullptr);
    _root_thread.detach();

    return true;
}

void Yggdrasil::stop() 
{
    {
        std::scoped_lock lock(_mutex);
        if (!_running.load(std::memory_order_acquire))
            return;

        _state.store(YggdrasilState::Stopping, std::memory_order_release);
        _lastMessage = kStoppingMsg;
    }
    notifyStatusChange();

    _root_running.store(false, std::memory_order_release);
    if (_root_thread.joinable()) {
        if (_root_thread.get_id() == std::this_thread::get_id()) {
            _root_thread.detach();
        } else {
            _root_thread.join();
        }
    }

    {
        std::scoped_lock lock(_mutex);
        _running.store(false, std::memory_order_release);
        _state.store(YggdrasilState::Stopped, std::memory_order_release);
        _lastMessage = kStopMsg;
    }
    notifyStatusChange();
}

void Yggdrasil::shutdown() {
    stop();

    {
        std::scoped_lock lock(_mutex);
        _initialized.store(false, std::memory_order_release);
        _state.store(YggdrasilState::Uninitialized, std::memory_order_release);
        _lastMessage = kShutdownMsg;
        _startTime = {};
        _tasksSubmitted.store(0, std::memory_order_relaxed);
        _tasksCompleted.store(0, std::memory_order_relaxed);
    }
    notifyStatusChange();
}

bool Yggdrasil::isInitialized() const noexcept {
    return _initialized.load(std::memory_order_acquire);
}

bool Yggdrasil::isRunning() const noexcept {
    return _running.load(std::memory_order_acquire);
}

YggdrasilState Yggdrasil::state() const noexcept {
    return _state.load(std::memory_order_acquire);
}

YggdrasilStatus Yggdrasil::status() const {
    std::scoped_lock lock(_mutex);
    return buildStatusUnsafe();
}

std::size_t Yggdrasil::addStatusListener(StatusListener listener) {
    if (!listener)
        return 0;

    std::scoped_lock lock(_mutex);
    const std::size_t token = ++_listenerToken;
    _listeners.emplace(token, std::move(listener));
    return token;
}

bool Yggdrasil::removeStatusListener(std::size_t token) {
    if (token == 0)
        return false;

    std::scoped_lock lock(_mutex);
    return _listeners.erase(token) > 0;
}

void Yggdrasil::signalTaskSubmitted(std::size_t count) {
    _tasksSubmitted.fetch_add(count, std::memory_order_relaxed);
    if (_config.trackMetrics)
        notifyStatusChange();
}

void Yggdrasil::signalTaskCompleted(std::size_t count) {
    _tasksCompleted.fetch_add(count, std::memory_order_relaxed);
    if (_config.trackMetrics)
        notifyStatusChange();
}

void Yggdrasil::notifyStatusChange() {
    YggdrasilStatus snapshot;
    std::vector<StatusListener> targets;
    {
        std::scoped_lock lock(_mutex);
        snapshot = buildStatusUnsafe();
        targets.reserve(_listeners.size());
        for (const auto& [_, listener] : _listeners) {
            if (listener)
                targets.push_back(listener);
        }
    }

    for (const auto& listener : targets) {
        if (listener)
            listener(snapshot);
    }
}

YggdrasilStatus Yggdrasil::buildStatusUnsafe() const {
    YggdrasilStatus status;
    status.state = _state.load(std::memory_order_acquire);
    status.initialized = _initialized.load(std::memory_order_acquire);
    status.running = _running.load(std::memory_order_acquire);
    status.name = _config.name;
    status.message = _lastMessage;
    status.timestamp = std::chrono::steady_clock::now();
    status.startTime = _startTime;
    status.tasksSubmitted = _tasksSubmitted.load(std::memory_order_relaxed);
    status.tasksCompleted = _tasksCompleted.load(std::memory_order_relaxed);
    return status;
}

} // namespace yggdrasil::platform
