#pragma once

#include <iostream>
#include <vector>
#include <atomic>

namespace Common {
    template <typename T>
    class LFQueue final {



        private:
            std::vector<T> store_;

            // size_t is used for indices and counters because it is the standard type
            // for sizes and memory indexing in C++; it is portable across architectures
            // (32/64-bit) and matches STL container sizes, avoiding truncation issues. 
            std::atomic<size_t> next_write_index_ = {0};
            std::atomic<size_t> next_read_index_ = {0};
            std::atomic<size_t> num_elements_ = {0};
    };
}