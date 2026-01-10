#pragma once

#include <cstdio>
#include <string>
#include <fstream>

#include "lf_queue.h"
#include "thread_utils.h"

namespace Common {
    constexpr size_t LOG_QUEUE_SIZE = 8 * 1024 * 1024;

    enum class LogType : int8_t {
        CHAR = 0,
        INTEGER = 1,
        LONG_INTEGER = 2,
        LONG_LONG_INTEGER = 3,
        UNSIGNED_INTEGER = 4,
        UNSIGNED_LONG_INTEGER = 5,
        UNSIGNED_LONG_LONG_INTEGER = 6,
        FLOAT = 7,
        DOUBLE = 8
    };

    struct LogElement {
        LogType type_ = LogType::CHAR;
        union {
            char c;
            int i;
            long li;
            long long lli;
            unsigned u;
            unsigned long ul;
            unsigned long long ull;
            float f;
            double d;
        };
    };

    class Logger final {

        auto flushQueue() noexcept{
            return 0;
        }

        explicit Logger (std::string &file_name) : file_name_(file_name), queue_(LOG_QUEUE_SIZE) {
            file_.open(file_name_);
            ASSERT(file_.is_open(), "Could not open log file: " + file_name_);

            logger_thread_ = createAndStartThread(-1, "Logger", [this]() { flushQueue(); } );
            ASSERT(logger_thread_ != nullptr, "Could not start logger thread.");
        }
        

    private:
        const std::string file_name_;
        std::ofstream file_;
        LFQueue<LogElement> queue_;
        std::atomic<bool> running_ = {true};
        std::thread *logger_thread_ = nullptr;
    };
}