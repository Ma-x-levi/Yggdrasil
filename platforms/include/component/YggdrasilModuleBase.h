#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <string_view>



/**
 * @brief Base interface for any unit of work that can be scheduled on the Yggdrasil thread pool.
 *
 * All concrete task types must inherit from this interface and implement the execution,
 * cancellation, and metadata accessors. The thread pool only depends on these contracts
 * when orchestrating tasks, enabling specialised implementations (blocking, async, I/O, etc.)
 * without coupling the pool to their details.
 */
class YggdrasilThreadBase {
public:
    using Ptr = std::shared_ptr<YggdrasilThreadBase>;
    using WeakPtr = std::weak_ptr<YggdrasilThreadBase>;

    static constexpr std::uint8_t kHighestPriority = 0;
    static constexpr std::uint8_t kLowestPriority = 255;
    static constexpr std::uint8_t kDefaultPriority = 128;

    virtual ~YggdrasilThreadBase() = default;

protected:
    static constexpr std::chrono::milliseconds kDefaultTimeout{0};
    static constexpr std::chrono::milliseconds kNoTimeout{kDefaultTimeout};
    static constexpr std::uint32_t kInvalidTaskId = 0U;
    static constexpr std::uint32_t kMaxRetryCount = 0U;
    static constexpr char kDefaultTaskName[] = "YggdrasilTask";

public:
    /**
     * @brief Execute the task's work payload.
     *
     * The thread pool invokes this on one of its worker threads.
     * Implementations should periodically consult @ref isCancellationRequested
     * to support cooperative cancellation.
     */
    virtual void execute() = 0;

    /**
     * @brief Request the task to cancel any outstanding work.
     *
     * This should be non-blocking and safe to call from any thread.
     * Implementations should set their cancellation state but must not throw.
     */
    virtual void cancel() noexcept = 0;

    /**
     * @brief Query whether cancellation has been requested.
     */
    virtual bool isCancellationRequested() const noexcept = 0;

    /**
     * @brief Human-readable task identifier used for diagnostics or logging.
     */
    virtual std::string_view name() const noexcept = 0;

    /**
     * @brief Relative execution priority used by the scheduler.
     *
     * Smaller values denote higher priority (0 is the highest priority).
     * Default implementation returns @ref kDefaultPriority.
     */
    virtual std::uint8_t priority() const noexcept { return kDefaultPriority; }

    /**
     * @brief Optional estimate of how long the task may occupy a worker thread.
     *
     * Thread pool implementations can use this hint for heuristics such as
     * load balancing. Returning zero conveys "no estimate".
     */
    virtual std::chrono::milliseconds estimatedDuration() const noexcept {
        return std::chrono::milliseconds::zero();
    }

    /**
     * @brief Hook invoked by the scheduler immediately before enqueuing.
     *
     * Override when a derived class needs to refresh state prior to execution.
     */
    virtual void onBeforeSchedule() {}

    /**
     * @brief Hook invoked after execution completes (whether or not it succeeded).
     *
     * @param completedSuccessfully Indicates whether the task finished without throwing.
     * The hook should remain noexcept and avoid throwing exceptions.
     */
    virtual void onAfterFinish(bool completedSuccessfully) noexcept {
        (void)completedSuccessfully;
    }
};


