#pragma once

#include <atomic>
#include <thread>

/**
 * @brief Minimal example backend service used to demonstrate the Yggdrasil runtime.
 *
 * The service exposes a simple lifecycle with @ref initialize, @ref start, and @ref stop
 * hooks. Internally it spawns a worker thread that emits a heartbeat message once per
 * second while the service is running.
 */
class BackendService {
public:
    BackendService();
    ~BackendService();

    BackendService(const BackendService&) = delete;
    BackendService& operator=(const BackendService&) = delete;

    [[nodiscard]] bool initialize();
    [[nodiscard]] bool start();
    void stop();
    void join();

    [[nodiscard]] bool isRunning() const noexcept;

private:
    void workerLoop();

    std::atomic<bool> _initialized;
    std::atomic<bool> _running;
    std::thread _worker;
};
