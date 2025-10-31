#include "YggdrasilThreadPool.h"

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <functional>
#include <utility>

namespace yggdrasil::platform {

YggdrasilThreadPool::YggdrasilThreadPool()
    : _nextId(1)
    , _running(false)
    , _initialized(false)
    , _workerCount(0) {}

YggdrasilThreadPool::~YggdrasilThreadPool() {
    shutdown();
}

bool YggdrasilThreadPool::initialize(std::size_t workerCount) {
    workerCount = std::max<std::size_t>(1, workerCount);

    bool expected = false;
    if (!_initialized.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
        return false;
    }

    _workerCount = workerCount;
    _running.store(true, std::memory_order_release);

    _workers.reserve(workerCount);
    for (std::size_t i = 0; i < workerCount; ++i) {
        _workers.emplace_back(&YggdrasilThreadPool::workerLoop, this, i);
    }

    return true;
}

void YggdrasilThreadPool::shutdown() {
    bool expected = true;
    if (!_initialized.compare_exchange_strong(expected, false, std::memory_order_acq_rel)) {
        return;
    }

    _running.store(false, std::memory_order_release);
    _taskCv.notify_all();

    for (auto& worker : _workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    _workers.clear();
    _workerCount = 0;

    {
        std::lock_guard lock(_taskMutex);
        while (!_taskQueue.empty()) {
            _taskQueue.pop();
        }
        for (auto& [id, record] : _tasks) {
            if (!record.cancelled && record.metrics.state == TaskState::Pending) {
                record.cancelled = true;
                record.metrics.state = TaskState::Cancelled;
            }
        }
    }

    std::vector<TaskId> loopIds;
    {
        std::lock_guard lock(_loopMutex);
        loopIds.reserve(_loopTasks.size());
        for (const auto& [id, record] : _loopTasks) {
            (void)record;
            loopIds.push_back(id);
        }
    }

    for (auto id : loopIds) {
        stopLoopTask(id);
    }
}

bool YggdrasilThreadPool::isInitialized() const noexcept {
    return _initialized.load(std::memory_order_acquire);
}

std::size_t YggdrasilThreadPool::workerCount() const noexcept {
    return _workerCount;
}

YggdrasilThreadPool::TaskId YggdrasilThreadPool::submit(YggdrasilThreadBase::Ptr task) {
    if (!task) {
        return kInvalidTaskId;
    }

    if (!_running.load(std::memory_order_acquire)) {
        return kInvalidTaskId;
    }

    const TaskId id = nextTaskId();

    {
        std::lock_guard lock(_taskMutex);
        TaskRecord record;
        record.task = std::move(task);
        record.metrics.id = id;
        record.metrics.state = TaskState::Pending;
        _tasks.emplace(id, std::move(record));
        _taskQueue.push(id);
    }

    _taskCv.notify_one();
    return id;
}

bool YggdrasilThreadPool::cancel(TaskId taskId) {
    if (taskId == kInvalidTaskId) {
        return false;
    }

    YggdrasilThreadBase::Ptr task;

    {
        std::lock_guard lock(_taskMutex);
        auto it = _tasks.find(taskId);
        if (it == _tasks.end()) {
            return false;
        }

        auto& record = it->second;
        if (record.metrics.state == TaskState::Completed ||
            record.metrics.state == TaskState::Failed ||
            record.metrics.state == TaskState::Cancelled) {
            return false;
        }

        record.cancelled = true;
        record.metrics.state = TaskState::Cancelled;
        task = record.task;
    }

    if (task) {
        task->cancel();
    }

    _taskCv.notify_all();
    return true;
}

std::optional<YggdrasilThreadPool::TaskMetrics> YggdrasilThreadPool::taskMetrics(TaskId taskId) const {
    if (taskId == kInvalidTaskId) {
        return std::nullopt;
    }

    std::lock_guard lock(_taskMutex);
    auto it = _tasks.find(taskId);
    if (it == _tasks.end()) {
        return std::nullopt;
    }

    return it->second.metrics;
}

YggdrasilThreadPool::TaskId YggdrasilThreadPool::startLoopTask(YggdrasilThreadBase::Ptr task) {
    if (!task) {
        return kInvalidTaskId;
    }

    const TaskId id = nextTaskId();
    auto record = std::make_shared<LoopTaskRecord>();
    record->id = id;
    record->task = std::move(task);
    record->metrics.id = id;
    record->metrics.state = TaskState::Pending;
    record->running.store(true, std::memory_order_release);
    record->stopRequested.store(false, std::memory_order_release);

    {
        std::lock_guard lock(_loopMutex);
        _loopTasks.emplace(id, record);
    }

    record->worker = std::thread(&YggdrasilThreadPool::loopTaskRunner, this, record);
    return id;
}

bool YggdrasilThreadPool::stopLoopTask(TaskId taskId) {
    if (taskId == kInvalidTaskId) {
        return false;
    }

    std::shared_ptr<LoopTaskRecord> record;
    {
        std::lock_guard lock(_loopMutex);
        auto it = _loopTasks.find(taskId);
        if (it == _loopTasks.end()) {
            return false;
        }
        record = it->second;
        _loopTasks.erase(it);
    }

    record->stopRequested.store(true, std::memory_order_release);
    if (record->task) {
        record->task->cancel();
    }

    if (record->worker.joinable()) {
        record->worker.join();
    }

    {
        std::lock_guard lock(record->mutex);
        if (record->metrics.state != TaskState::Failed) {
            record->metrics.state = TaskState::Stopped;
        }
    }

    record->running.store(false, std::memory_order_release);
    return true;
}

std::optional<YggdrasilThreadPool::TaskMetrics> YggdrasilThreadPool::loopTaskMetrics(TaskId taskId) const {
    if (taskId == kInvalidTaskId) {
        return std::nullopt;
    }

    std::shared_ptr<LoopTaskRecord> record;
    {
        std::lock_guard lock(_loopMutex);
        auto it = _loopTasks.find(taskId);
        if (it == _loopTasks.end()) {
            return std::nullopt;
        }
        record = it->second;
    }

    std::lock_guard lock(record->mutex);
    return record->metrics;
}

void YggdrasilThreadPool::workerLoop(std::size_t /*workerIndex*/) {
    while (true) {
        TaskId taskId = kInvalidTaskId;
        YggdrasilThreadBase::Ptr task;
        std::chrono::steady_clock::time_point startTime;

        {
            std::unique_lock lock(_taskMutex);
            _taskCv.wait(lock, [this] {
                return !_taskQueue.empty() || !_running.load(std::memory_order_acquire);
            });

            if (_taskQueue.empty() && !_running.load(std::memory_order_acquire)) {
                break;
            }

            if (_taskQueue.empty()) {
                continue;
            }

            taskId = _taskQueue.front();
            _taskQueue.pop();

            auto it = _tasks.find(taskId);
            if (it == _tasks.end()) {
                continue;
            }

            auto& record = it->second;
            if (record.cancelled) {
                record.metrics.state = TaskState::Cancelled;
                continue;
            }

            task = record.task;
            startTime = std::chrono::steady_clock::now();
            record.metrics.state = TaskState::Running;
            record.metrics.lastStart = startTime;
            record.metrics.lastError.clear();
        }

        if (!task) {
            continue;
        }

        bool success = false;
        std::string errorMessage;

        try {
            task->onBeforeSchedule();
            task->execute();
            success = true;
        } catch (const std::exception& ex) {
            errorMessage = ex.what();
        } catch (...) {
            errorMessage = "unknown exception";
        }

        try {
            task->onAfterFinish(success);
        } catch (...) {
            // Swallow exceptions from the post-run hook to avoid crashing the pool.
        }

        const auto finishTime = std::chrono::steady_clock::now();

        {
            std::lock_guard lock(_taskMutex);
            auto it = _tasks.find(taskId);
            if (it == _tasks.end()) {
                continue;
            }

            auto& record = it->second;
            record.metrics.runCount += 1;
            record.metrics.lastFinish = finishTime;
            record.metrics.lastDuration = finishTime - startTime;

            if (record.cancelled) {
                record.metrics.state = TaskState::Cancelled;
                record.metrics.lastError.clear();
            } else if (success) {
                record.metrics.state = TaskState::Completed;
                record.metrics.lastError.clear();
            } else {
                record.metrics.state = TaskState::Failed;
                record.metrics.lastError = std::move(errorMessage);
            }

            if (record.metrics.state == TaskState::Completed ||
                record.metrics.state == TaskState::Failed ||
                record.metrics.state == TaskState::Cancelled) {
                record.task.reset();
            }
        }
    }
}

void YggdrasilThreadPool::loopTaskRunner(const std::shared_ptr<LoopTaskRecord>& record) {
    while (!record->stopRequested.load(std::memory_order_acquire)) {
        auto task = record->task;
        if (!task) {
            break;
        }

        const auto startTime = std::chrono::steady_clock::now();
        {
            std::lock_guard lock(record->mutex);
            record->metrics.state = TaskState::Running;
            record->metrics.lastStart = startTime;
            record->metrics.lastError.clear();
        }

        bool success = false;
        std::string errorMessage;

        try {
            task->onBeforeSchedule();
            task->execute();
            success = true;
        } catch (const std::exception& ex) {
            errorMessage = ex.what();
        } catch (...) {
            errorMessage = "unknown exception";
        }

        try {
            task->onAfterFinish(success);
        } catch (...) {
            // Ignore hook failures for loop tasks as well.
        }

        const auto finishTime = std::chrono::steady_clock::now();

        {
            std::lock_guard lock(record->mutex);
            record->metrics.runCount += 1;
            record->metrics.lastFinish = finishTime;
            record->metrics.lastDuration = finishTime - startTime;

            if (record->stopRequested.load(std::memory_order_acquire)) {
                record->metrics.state = success ? TaskState::Stopped : TaskState::Failed;
                if (!success) {
                    record->metrics.lastError = std::move(errorMessage);
                }
            } else if (success) {
                record->metrics.state = TaskState::Idle;
                record->metrics.lastError.clear();
            } else {
                record->metrics.state = TaskState::Failed;
                record->metrics.lastError = std::move(errorMessage);
            }
        }

        if (!success) {
            break;
        }

        std::this_thread::yield();
    }

    record->running.store(false, std::memory_order_release);
}

YggdrasilThreadPool::TaskId YggdrasilThreadPool::nextTaskId() {
    return _nextId.fetch_add(1, std::memory_order_relaxed);
}

namespace {

std::size_t resolveWorkerCountFromArgs(int argc, char* argv[]) {
    if (argc > 1 && argv != nullptr) {
        char* end = nullptr;
        const unsigned long parsed = std::strtoul(argv[1], &end, 10);
        if (end != nullptr && *end == '\0' && parsed > 0) {
            return static_cast<std::size_t>(parsed);
        }
    }

    const auto hw = std::thread::hardware_concurrency();
    return hw == 0 ? std::size_t{1} : static_cast<std::size_t>(hw);
}

int runThreadPoolEntry(int argc, char* argv[]) {
    YggdrasilThreadPool pool;
    const auto workers = resolveWorkerCountFromArgs(argc, argv);
    if (!pool.initialize(workers)) {
        return -1;
    }

    // Hook for user-provided workload bound through std::function or std::bind.
    pool.shutdown();
    return 0;
}

} // namespace

int main(int argc, char* argv[]) {
    auto entry = std::function<int(int, char**)>(runThreadPoolEntry);
    return entry(argc, argv);
}

} // namespace yggdrasil::platform
