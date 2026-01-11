#pragma once

#include <vector>
#include <atomic>

// SPSC Buffer - lossy queue
template <typename T>
class RingBuffer final {
    private:
        std::vector<T> buffer_;
        std::atomic<size_t> size_ = {0};
        std::atomic<size_t> read_index_ = {0};
        std::atomic<size_t> write_index_ = {0};

        auto is_full() noexcept {
            auto next_write = (write_index_ + 1) % buffer_.size();
            return read_index_ == next_write;
        }

    public:
        explicit RingBuffer(size_t size) : buffer_(size, T()) {
        };

        // Returns pointer to next element to read from buffer
        auto pop_next() const noexcept -> const T* {
            // we allow to override element
            return &buffer_[read_index_];
        }

        // Moves read pointer to the next element in buffer
        auto move_read_index() noexcept {
            read_index_ = (read_index_ + 1) % buffer_.size();
            size_--;
        }

        // Returns pointer to next element to write to
        T* push_into() noexcept {
            return &buffer_[write_index_];
        }

        // Moves write pointer to next place in buffer
        auto move_write_index() noexcept {
            write_index_ = (write_index_ + 1) % buffer_.size();
            size_++;
        }

        // Returns size of the RingBuffer
        auto size() noexcept {
            return size_.load();
        }

        // Boilerplate
        RingBuffer() = delete;
        RingBuffer(const RingBuffer&) = delete;
        RingBuffer(const RingBuffer&&) = delete;
        RingBuffer& operator=(const RingBuffer&) = delete;
        RingBuffer& operator=(const RingBuffer&&) = delete;
}; 