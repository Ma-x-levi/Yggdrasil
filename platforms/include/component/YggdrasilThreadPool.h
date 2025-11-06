#pragma once

#include "YggdrasilModuleBase.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>



/**
 * @brief Cooperative thread pool that schedules @ref YggdrasilThreadBase tasks.
 *
 * The pool manages two execution models:
 *   1) One-shot tasks executed on a finite worker pool controlled by @ref initialize.
 *   2) Looping tasks executed on dedicated threads created per registration.
 *
 * Both execution models expose lightweight metrics so callers can inspect task state,
 * duration of the last run, and the number of executions.
 */
class YggdrasilThreadPool final {
public:
    using TaskId = std::size_t;
    using TaskInterface = std::function<int(int, char* )>;

    static constexpr TaskId kInvalidTaskId = 0;

    enum class TaskState : std::uint8_t {
        Pending = 0,
        Running,
        Idle,
        Completed,
        Cancelled,
        Failed,
        Stopped
    };

    struct TaskMetrics {
        TaskId id{kInvalidTaskId};
        TaskState state{TaskState::Pending};
        std::size_t runCount{0};
        std::chrono::steady_clock::time_point lastStart{};
        std::chrono::steady_clock::time_point lastFinish{};
        std::chrono::nanoseconds lastDuration{std::chrono::nanoseconds::zero()};
        std::string lastError;
    };

    YggdrasilThreadPool();
    ~YggdrasilThreadPool();

    /**
     * @brief Create worker threads for single-shot task execution.
     *
     * @param workerCount Number of worker threads to spawn. Values less than 1
     * default to 1.
     * @return true when the pool transitions from the stopped state to running.
     * @return false if the pool was already initialized.
     */
    [[nodiscard]] bool initialize(std::size_t workerCount);

    /**
     * @brief Stop workers and cancel outstanding tasks.
     */
    void shutdown();

    [[nodiscard]] bool isInitialized() const noexcept;
    [[nodiscard]] std::size_t workerCount() const noexcept;

    /**
     * @brief Submit a single-run task to the worker pool.
     *
     * @return Task identifier, or kInvalidTaskId if the pool is not running.
     */
    [[nodiscard]] TaskId submit(YggdrasilThreadBase::Ptr task);

    /**
     * @brief Attempt to cancel a task that has not yet completed.
     *
     * If the task is currently running, @ref YggdrasilThreadBase::cancel will be
     * invoked cooperatively.
     */
    bool cancel(TaskId taskId);

    /**
     * @brief Retrieve metrics for a single-shot task.
     */
    [[nodiscard]] std::optional<TaskMetrics> taskMetrics(TaskId taskId) const;

    /**
     * @brief Register a looping task that executes on its own dedicated thread.
     *
     * The task will repeatedly call @ref YggdrasilThreadBase::execute until stopped.
     */
    [[nodiscard]] TaskId startLoopTask(YggdrasilThreadBase::Ptr task);

    /**
     * @brief Request shutdown of a looping task.
     */
    bool stopLoopTask(TaskId taskId);

    /**
     * @brief Retrieve metrics for a looping task.
     */
    [[nodiscard]] std::optional<TaskMetrics> loopTaskMetrics(TaskId taskId) const;

private:
    struct TaskRecord {
        YggdrasilThreadBase::Ptr task;
        TaskMetrics metrics;
        bool cancelled{false};
    };

    struct LoopTaskRecord {
        TaskId id{kInvalidTaskId};
        YggdrasilThreadBase::Ptr task;
        std::thread worker;
        std::atomic<bool> running{false};
        std::atomic<bool> stopRequested{false};
        mutable std::mutex mutex;
        TaskMetrics metrics;
    };

    void workerLoop(std::size_t workerIndex);
    void loopTaskRunner(const std::shared_ptr<LoopTaskRecord>& record);
    TaskId nextTaskId();

    std::atomic<TaskId> _nextId;
    std::vector<std::thread> _workers;

    mutable std::mutex _taskMutex;
    std::condition_variable _taskCv;
    std::queue<TaskId> _taskQueue;
    std::unordered_map<TaskId, TaskRecord> _tasks;

    mutable std::mutex _loopMutex;
    std::unordered_map<TaskId, std::shared_ptr<LoopTaskRecord>> _loopTasks;

    std::atomic<bool> _running;
    std::atomic<bool> _initialized;
    std::size_t _workerCount;
};


