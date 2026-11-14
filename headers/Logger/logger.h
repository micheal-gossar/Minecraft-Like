/** @file
 * @brief Synchronized hierarchical console and file logging.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <cstddef>
#include <fstream>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

/// Message severity controlling its label, color, and console destination.
enum class LogLevel {
    Debug, ///< Diagnostic output, omitted unless SAILING_DEBUG is defined.
    Info, ///< Normal informational output to stdout.
    Warning, ///< Recoverable issue reported to stderr.
    Error, ///< Failure reported to stderr.
};

class Logger;

/// Lightweight named logging handle; its owning Logger must outlive every copy.
class LogStream {
public:
    /// Creates or reuses a child category; rejects empty names or names containing '/'.
    [[nodiscard]] LogStream Child(std::string name) const;

    /// Writes a debug message when debug logging is enabled.
    void Debug(std::string_view message) const;
    /// Writes an informational message.
    void Info(std::string_view message) const;
    /// Writes a warning message.
    void Warning(std::string_view message) const;
    /// Writes an error message.
    void Error(std::string_view message) const;

    /// Returns the full category path; the view is valid while this handle is unchanged.
    [[nodiscard]] std::string_view Name() const { return path; }

private:
    friend class Logger;

    LogStream(Logger& owner, std::string streamPath);

    Logger* logger;
    std::string path;
};

/**
 * @brief Owns synchronized console output, stream registration, and a timestamped log file.
 * @note Logs are relative to the working directory. Calls are mutex-protected;
 * destruction still requires all users of borrowed LogStream handles to have stopped.
 */
class Logger {
public:
    /// Opens a file in logFile and removes recognized logs older than 30 days; throws on failure.
    Logger();
    /// Closes the log file after all borrowed streams are no longer in use.
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /// Creates or reuses a root category; rejects empty names or names containing '/'.
    [[nodiscard]] LogStream CreateStream(std::string name);
    /**
     * @brief Writes a timestamped message to the console and file.
     * @param message Text to write.
     * @param level Message severity; debug messages can be compiled out.
     * @param streamName Existing full category path created by CreateStream() or LogStream::Child().
     * @throws std::invalid_argument If an emitted message has an unknown category or severity.
     */
    void Log(std::string_view message, LogLevel level, std::string_view streamName);
    /// Flushes both console streams and the log file under the logger mutex.
    void Flush();

private:
    friend class LogStream;

    struct StreamDefinition {
        std::string name;
        std::string parentPath;
        std::size_t depth;
    };

    [[nodiscard]] LogStream CreateChildStream(std::string_view parentPath, std::string name);
    [[nodiscard]] std::string FormatCategory(std::string_view streamName) const;

    mutable std::mutex mutex;
    std::unordered_map<std::string, StreamDefinition> streams;
    std::ofstream logFile;
};

#endif
