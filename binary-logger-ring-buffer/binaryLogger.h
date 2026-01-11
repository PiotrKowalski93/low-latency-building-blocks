#pragma once

#include <iostream>


class BinaryLogger final {
    private:
        std::string file_name_;


    public:
        auto log(std::string &str) noexcept {
            //TODO: use mmap and memcpy
        }

};