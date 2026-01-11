#include <iostream>

#include "ringBuffer.h"

int main()
{
    RingBuffer<int> buffer(5);

    // std::cout << "Size: " << buffer.size() << std::endl;
    // int x = 1;
    // *buffer.push_into() = x;
    // buffer.move_write_index();
    // std::cout << "Mem: " << buffer.pop_next() << ", Value: " << *buffer.pop_next() << std::endl;
    // buffer.move_read_index();

    for (size_t i = 0; i < 7; i++)
    {
        *buffer.push_into() = i;
        buffer.move_write_index();
    }
    std::cout << "Size: " << buffer.size() << std::endl;

    for (size_t i = 0; i < 3; i++)
    {
        std::cout << "Mem: " << buffer.pop_next() << ", Value: " << *buffer.pop_next() << std::endl;
        buffer.move_read_index();
    }
    std::cout << "Size: " << buffer.size() << std::endl;

    return 0;
}
