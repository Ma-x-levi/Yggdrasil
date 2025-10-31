#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <cstdio>
#include <functional>
#include <iomanip>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

namespace yggdrasil::platform {

/**
 * @brief Base interface for framework logging.
 *
 * Derive from this class to provide concrete logging behaviour such as
 * printing to console or persisting to storage. The base handles severity
 * filtering, record construction, and optional sink fan-out so that
 * subclasses can focus on the actual output implementation.
 */
class YggdrasilLoggerBase {
public:
    enum class Level : std::uint8_t {
        Trace = 0,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    struct Record {
        Level level{Level::Info};
        std::string message;
        std::string category;
        std::chrono::system_clock::time_point timestamp{};
        std::thread::id threadId{};
    };

    using LogSink = std::function<void(const Record&)>;
    using Ptr = std::shared_ptr<YggdrasilLoggerBase>;

    virtual ~YggdrasilLoggerBase() = default;

    static constexpr std::string_view toString(Level level) noexcept {
        switch (level) {
        case Level::Trace:
            return "TRACE";
        case Level::Debug:
            return "DEBUG";
        case Level::Info:
            return "INFO";
        case Level::Warn:
            return "WARN";
        case Level::Error:
            return "ERROR";
        case Level::Critical:
            return "CRITICAL";
        }
        return "UNKNOWN";
    }

    [[nodiscard]] static Level levelFromString(std::string_view text, Level fallback = Level::Info) noexcept {
        if (text == "TRACE" || text == "trace")
            return Level::Trace;
        if (text == "DEBUG" || text == "debug")
            return Level::Debug;
        if (text == "INFO" || text == "info")
            return Level::Info;
        if (text == "WARN" || text == "warn" || text == "warning" || text == "WARNING")
            return Level::Warn;
        if (text == "ERROR" || text == "error")
            return Level::Error;
        if (text == "CRITICAL" || text == "critical" || text == "fatal" || text == "FATAL")
            return Level::Critical;
        return fallback;
    }

    void log(Level level, std::string_view message) {
        log(level, message, {});
    }

    void log(Level level, std::string_view message, std::string_view category) {
        Record record;
        record.level = level;
        record.message.assign(message.begin(), message.end());
        record.category.assign(category.begin(), category.end());
        record.timestamp = std::chrono::system_clock::now();
        record.threadId = std::this_thread::get_id();
        log(std::move(record));
    }

    void log(Record record) {
        if (!isEnabled(record.level)) {
            return;
        }

        if (record.timestamp.time_since_epoch().count() == 0) {
            record.timestamp = std::chrono::system_clock::now();
        }
        if (record.threadId == std::thread::id{}) {
            record.threadId = std::this_thread::get_id();
        }

        dispatch(record);
    }

    void trace(std::string_view message, std::string_view category = {}) {
        log(Level::Trace, message, category);
    }

    void debug(std::string_view message, std::string_view category = {}) {
        log(Level::Debug, message, category);
    }

    void info(std::string_view message, std::string_view category = {}) {
        log(Level::Info, message, category);
    }

    void warn(std::string_view message, std::string_view category = {}) {
        log(Level::Warn, message, category);
    }

    void error(std::string_view message, std::string_view category = {}) {
        log(Level::Error, message, category);
    }

    void critical(std::string_view message, std::string_view category = {}) {
        log(Level::Critical, message, category);
    }

    virtual void flush() {
        try {
            handleFlush();
        } catch (...) {
            // Never allow logging failures to propagate into the runtime.
        }
    }

    [[nodiscard]] bool isEnabled(Level level) const noexcept {
        return static_cast<std::uint8_t>(level) >= static_cast<std::uint8_t>(_minimumLevel.load(std::memory_order_acquire));
    }

    void setLevel(Level level) noexcept {
        _minimumLevel.store(level, std::memory_order_release);
    }

    [[nodiscard]] Level level() const noexcept {
        return _minimumLevel.load(std::memory_order_acquire);
    }

    void addSink(LogSink sink) {
        if (!sink) {
            return;
        }

        std::lock_guard<std::mutex> lock(_sinkMutex);
        _sinks.push_back(std::move(sink));
    }

    void clearSinks() {
        std::lock_guard<std::mutex> lock(_sinkMutex);
        _sinks.clear();
    }

    [[nodiscard]] static std::string formatRecord(const Record& record) {
        std::ostringstream oss;

        auto timestamp = record.timestamp;
        if (timestamp.time_since_epoch().count() == 0) {
            timestamp = std::chrono::system_clock::now();
        }

        const auto timeT = std::chrono::system_clock::to_time_t(timestamp);
        std::tm timeInfo{};
#if defined(_WIN32)
        localtime_s(&timeInfo, &timeT);
#else
        localtime_r(&timeT, &timeInfo);
#endif

        char timeBuffer[32];
        if (std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo) == 0) {
            std::snprintf(timeBuffer, sizeof(timeBuffer), "1970-01-01 00:00:00");
        }

        const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(
            timestamp.time_since_epoch()
        ).count() % 1000000LL;

        oss << timeBuffer << '.'
            << std::setw(6) << std::setfill('0') << micros;
        oss << " [" << toString(record.level) << ']';
        if (!record.category.empty()) {
            oss << " {" << record.category << '}';
        }
        oss << " [tid=" << record.threadId << "] ";
        oss << record.message;
        return oss.str();
    }

protected:
    virtual void handleRecord(const Record& record) = 0;
    virtual void handleFlush() {}

private:
    void dispatch(const Record& record) {
        try {
            handleRecord(record);
        } catch (...) {
            // Ignore backend failures to keep the caller unharmed.
        }

        std::vector<LogSink> sinksCopy;
        {
            std::lock_guard<std::mutex> lock(_sinkMutex);
            sinksCopy = _sinks;
        }

        for (auto& sink : sinksCopy) {
            if (!sink) {
                continue;
            }

            try {
                sink(record);
            } catch (...) {
                // Sink failures should not interfere with the logging pipeline.
            }
        }
    }

    std::atomic<Level> _minimumLevel{Level::Info};
    mutable std::mutex _sinkMutex;
    std::vector<LogSink> _sinks;
};

} // namespace yggdrasil::platform
