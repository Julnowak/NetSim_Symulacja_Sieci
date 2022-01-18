//
// Created by Julia on 19.12.2021.
//

#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"
#include "types.hpp"
#include <list>


enum class PackageQueueType{
    FIFO, LIFO
};


class IPackageStockpile{
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&&) = 0;
    virtual bool empty() = 0;
    virtual size_type size() = 0;    // what if we use size_t?

    // iteratory
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;     // Wirtualny destruktor

};


class IPackageQueue : public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;
};


class PackageQueue : public IPackageQueue {
public:
    std::list<Package> queue;
    PackageQueueType queueType;

    explicit PackageQueue(PackageQueueType type) { queueType = type;};

    // Po pierwszej klasie
    void push(Package&& pack) override { queue.emplace_back(std::move(pack)); };
    bool empty() override { return queue.empty();};
    size_type size() override { return size_type(queue.size());};

    const_iterator begin() const override { return queue.begin();};
    const_iterator end() const override{ return queue.end();};
    const_iterator cbegin() const override { return queue.cbegin();};
    const_iterator cend() const override { return queue.cend();};

    // Po drugiej klasie
    Package pop() override;
    PackageQueueType get_queue_type() override { return queueType;};
};

#endif //NETSIM_STORAGE_TYPES_HPP
