#include <iostream>

#include "lf_queue.h"
#include "thread_utils.h"

struct MyStruct{
    int d_[3];
};

using namespace Common;

auto consumeFunction(LFQueue<MyStruct>* lfq){
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5s);

    while(lfq->size()) {
        const auto item = lfq->getNextToRead();
        lfq->updateReadIndex();

        std::cout << "Consume Func got:" << item->d_[0] << "," << item->d_[1] << "," << item->d_[2] << " | queue size: " << lfq->size() << std::endl;
        std::this_thread::sleep_for(1s);
    }

    std::cout << "Consume Func exiting" << std::endl;
}

auto writeFunction(LFQueue<MyStruct>* lfq){
    for (auto i = 0; i < 50; ++i)
    {
        const MyStruct item{i, i*10, i*100};
        *(lfq->getNextToWriteTo()) = item;
        lfq->updateNextToWriteTo();

        std::cout << "Write Func:" << item.d_[0] << "," << item.d_[1] << "," << item.d_[2] << " | queue size: " << lfq->size() << std::endl;

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
}

int main()
{
    LFQueue<MyStruct> lfq(20);

    // In low-latency consumer does not have to be pinned often
    // hance -1, -1 = leave this thread to the scheduler
    auto consumer_thread = createAndStartThread(-1, "consumer", consumeFunction, &lfq);

    // In low-latency writer/producer is more critical that is why we pin it to CPU #1
    auto writer_thread = createAndStartThread(1, "writer", writeFunction, &lfq);
        
    consumer_thread->join();
    writer_thread->join();

    std::cout << "main exiting." << std::endl;
    return 0;
}
