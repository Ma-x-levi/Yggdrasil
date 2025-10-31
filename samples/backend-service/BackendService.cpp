#include "BackendService.h"

#include <chrono>
#include <iostream>
#include <thread>

namespace {
constexpr std::chrono::seconds kHeartbeatInterval{1};
} // namespace

BackendService::BackendService()
    : _initialized(false)
    , _running(false) {}

BackendService::~BackendService() {
    stop();
    join();
}

bool BackendService::initialize() {
    bool expected = false;
    if (_initialized.compare_exchange_strong(expected, true)) {
        std::cout << "[BackendService] Initialized successfully." << std::endl;
    } else {
        std::cout << "[BackendService] Already initialized, skipping." << std::endl;
    }
    return true;
}

bool BackendService::start() {
    if (!_initialized.load(std::memory_order_acquire)) {
        std::cerr << "[BackendService] Cannot start before initialization." << std::endl;
        return false;
    }

    bool expected = false;
    if (!_running.compare_exchange_strong(expected, true)) {
        std::cerr << "[BackendService] Service already running." << std::endl;
        return false;
    }

    std::cout << "[BackendService] Starting worker thread." << std::endl;

    _worker = std::thread(&BackendService::workerLoop, this);
    return true;
}

void BackendService::stop() {
    bool wasRunning = _running.exchange(false);
    if (wasRunning) {
        std::cout << "[BackendService] Stop requested." << std::endl;
    }
}

void BackendService::join() {
    if (_worker.joinable()) {
        _worker.join();
        std::cout << "[BackendService] Worker terminated." << std::endl;
    }
}

bool BackendService::isRunning() const noexcept {
    return _running.load(std::memory_order_relaxed);
}

void BackendService::workerLoop() {
    std::size_t heartbeat = 0;
    while (_running.load(std::memory_order_relaxed)) {
        ++heartbeat;
        std::cout << "[BackendService] Heartbeat #" << heartbeat << std::endl;
        std::this_thread::sleep_for(kHeartbeatInterval);
    }
    std::cout << "[BackendService] Worker shutting down gracefully." << std::endl;
}
