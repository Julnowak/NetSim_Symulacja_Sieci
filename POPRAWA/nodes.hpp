//
// Created by Julia on 01.01.2022.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"
#include "config.hpp"

#include <iostream>
#include <memory>
#include <optional>
#include <map>
#include <functional>



enum ReceiverType{
    WORKER, STOREHOUSE
};


class IPackageReceiver{
public:
    using const_it = IPackageStockpile::const_iterator;

    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;

#if (EXERCISE_ID > EXERCISE_ID_NODES)
    virtual ReceiverType get_receiver_type () const = 0;
#endif

    // iteratory
    virtual const_it begin() const = 0;
    virtual const_it end() const = 0;
    virtual const_it cbegin() const = 0;
    virtual const_it cend() const = 0;

};


class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : _pg(std::move(pg)) {};

    void add_receiver (IPackageReceiver* r);
    void remove_receiver (IPackageReceiver* r);
    IPackageReceiver *choose_receiver();
    const preferences_t & get_preferences() const {return preferences;};

    // Iteratory
    const_iterator begin() const {return preferences.begin();}
    const_iterator end() const {return preferences.end();}
    const_iterator cbegin() const {return preferences.cbegin();}
    const_iterator cend() const {return preferences.cend();}

private:
    ProbabilityGenerator _pg;
    preferences_t preferences;

};

class PackageSender{
public:
    //PackageSender(PackageSender&&);
    void send_package();
    const std::optional<Package>& get_sending_buffer() const {return bufor;}
    ReceiverPreferences receiver_preferences_;

protected:
    void push_package(Package&& pack) {bufor = std::move(pack);};

private:
    std::optional<Package> bufor;
};


class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::LIFO)) : _id(id), _d(std::move(d)) {}

#if (EXERCISE_ID > EXERCISE_ID_NODES)
    ReceiverType get_receiver_type() const {return ReceiverType::STOREHOUSE;};
#endif

    ElementID get_id() const { return _id; }
    void receive_package(Package&& pack) { _d->push(std::move(pack)); }

    // iteratory
    const_it begin() const {return _d -> begin();};
    const_it end() const {return _d -> end();};
    const_it cbegin() const {return _d -> cbegin();};
    const_it cend() const {return _d -> cend();};

private:
    ElementID _id;
    std::unique_ptr<IPackageStockpile> _d;
};


class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di): _id(id), _di(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const {return _di;}
    ElementID get_id() const {return _id;}

private:
    ElementID _id;
    TimeOffset _di;
};


class Worker: public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : _id(id), _pd(pd), _q(std::move(q)) {}
    void do_work(Time t);
    TimeOffset get_processing_duration() const {return _pd;}
    Time get_package_processing_start_time() const {return _t;}

    // Do Factory
#if (EXERCISE_ID > EXERCISE_ID_NODES)
    ReceiverType get_receiver_type() const override {return ReceiverType::WORKER;}
#endif

    // Interfejs
    void receive_package(Package&& p) override {_q->push(std::move(p));}
    ElementID get_id() const override { return _id; }

    // Iteratory
    IPackageStockpile::const_iterator begin() const override {return _q->begin();}
    IPackageStockpile::const_iterator end() const override {return _q->end();}
    IPackageStockpile::const_iterator cbegin() const override {return _q->cbegin();}
    IPackageStockpile::const_iterator cend() const override {return _q->cend();}

private:
    ElementID _id;
    TimeOffset _pd;
    std::unique_ptr<IPackageQueue> _q;
    Time _t = 0;
    std::optional<Package> _processing_buffer;
};


#endif //NETSIM_NODES_HPP
