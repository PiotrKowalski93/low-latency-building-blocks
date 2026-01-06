#include <cstddef>
#include <iostream>

#include "mem_pool.h"

struct MyStruct{
	int d_[3];
};

// Memory pool test and example
int main()
{
	using namespace Common;

	MemPool<double> prim_pool(50);
	MemPool<MyStruct> struct_pool(50);

	for(int i = 0; i < 50; ++i){
		auto p_ptr = prim_pool.allocate(i);
		auto s_ptr = struct_pool.allocate(MyStruct{i, i+1, i+2});

		std::cout << "prim elem: " << *p_ptr << " allocated at: " << p_ptr << std::endl;
		std::cout << "struct elem: [" << s_ptr->d_[0] << "," << s_ptr->d_[1] << "," << s_ptr->d_[2] << "] allocated at: " << s_ptr << std::endl;

		if(i % 5 == 0){
			std::cout << "deallocating prim elem: " << *p_ptr << " from: " << p_ptr << std::endl;
			prim_pool.deallocate(p_ptr);

			std::cout << "deallocating struct elem: [" << s_ptr->d_[0] << "," << s_ptr->d_[1] << "," << s_ptr->d_[2] << "] from: " << s_ptr << std::endl;
			struct_pool.deallocate(s_ptr);
		}
	}

	return 0;
}

//How to use ptr substraction
auto ptrDiff_example() {
	const std::size_t N = 10;
    int* a = new int[N];
    int* end = a + N;

	int* elem = &a[6];

	std::cout << elem - a << "\n";

    for (std::ptrdiff_t i = N; i > 0; --i){
		
        std::cout << end << "-" << i << "=" << (end - i) << ' ' << std::endl;
	}
		
    std::cout << '\n';
    delete[] a;
}
