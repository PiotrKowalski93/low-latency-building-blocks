#pragma once

// For file descriptors
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>

#include <cstdint>
#include <cstring>
#include <iostream>

//TODO: Add use for LogElement
class BinaryLogger final {
    private:
        size_t      file_size_;
        size_t      write_pos_;
        int         fd_;        // File descriptor
        uint8_t*    mmap_ptr_;  // Memory poiner

    public:
        explicit BinaryLogger(const char* file) :
            file_size_(1024*1024), // 1MB log file
            mmap_ptr_(nullptr),
            fd_(-1),
            write_pos_(0)
        {
            // open() returns int 
            // Each linux process contains list of open files (0,1,2,3....)
            // fd - are indexes in this list, so this int means "number that identifies my opened file"
            // O_RDWR - I want to read and write to this file
            // O_CREAT - if file does not exist, create it
            // 0644 - UNIX access 0, 6 - owner (write+read), 4 - group (read), 4 - others (read)
            fd_ = open(file, O_RDWR | O_CREAT, 0644);

            // Set up file size (does not work on 0bytes)
            // gives 1MB to the file (it is empty logically)
            if (ftruncate(fd_, file_size_) != 0) {
                close(fd_);
            }

            // We need to add desciptor - "Do memory maping for the file under this descriptor"
            // PROT_READ | PROT_WRITE i need to read and write to this memory
            // MAP_SHARED ???
            void* addr = mmap(nullptr, file_size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);

            if (addr == MAP_FAILED) {
                // After closing fd_, mapping stil works
                // mmap and fd has two different lifecycles
                close(fd_);
            }

            // Map to bytes
            // mmap if succeded return addres, we need to cast it
            // to be able to do poiner arythemic 
            mmap_ptr_ = static_cast<uint8_t*>(addr);
        }

        ~BinaryLogger() {
            if (mmap_ptr_) {
                munmap(mmap_ptr_, file_size_);
            }

            if (fd_ >= 0) {
                close(fd_);
            }
        }

        // TODO: Add rolling files
        // TODO: Change format from [uint32 len][bytes...] to smth like [magic][version][timestamp][len][payload][crc]
        auto log(std::string str) noexcept {
            const uint32_t len = static_cast<uint32_t>(str.size());

            // Rolling file cdn...
            if (write_pos_ + sizeof(len) + len > file_size_) {
                // create new file
                return; 
            }

            // Save lenght
            std::memcpy(mmap_ptr_ + write_pos_, &len, sizeof(len));
            write_pos_ += sizeof(len);

            // Save data
            std::memcpy(mmap_ptr_ + write_pos_, str.data(), len);
            write_pos_ += len;
        }
};