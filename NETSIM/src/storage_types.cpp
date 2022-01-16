//
// Created by Julia on 19.12.2021.
//

#include "storage_types.hpp"

Package PackageQueue::pop()
{
    Package package;
    switch(queueType)
    {
        case PackageQueueType::FIFO:                // samo FIFO
        {
            package = std::move(queue.front());
            queue.pop_front();
            break;
        }
        case PackageQueueType::LIFO:                // samo LIFO
        {
            package = std::move(queue.back());
            queue.pop_back();
            break;
        }
    }
    return package;
}