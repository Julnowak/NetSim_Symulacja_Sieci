//
// Created by Julia on 01.01.2022.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"

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
    virtual ReceiverType get_receiver_type () = 0;

    // iteratory
    virtual const_it begin() const = 0;
    virtual const_it end() const = 0;
    virtual const_it cbegin() const = 0;
    virtual const_it cend() const = 0;
};


class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::LIFO)) : _id(id), _d(std::move(d)) {}

    ReceiverType get_receiver_type() {return ReceiverType::STOREHOUSE;};
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


class ReceiverPreferences{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    using reverse_iterator = preferences_t::reverse_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = default_probability_generator): _pg(pg) {}
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    const preferences_t& get_preferences() const {return preferences;}

    const_iterator begin() const {return preferences.cbegin();}
    const_iterator end() const {return preferences.cend();}
    const_iterator cbegin() const {return preferences.cbegin();}
    const_iterator cend() const {return preferences.cend();}
    reverse_iterator rbegin() {return preferences.rbegin();}
    reverse_iterator rend() {return preferences.rend();}

private:
    preferences_t preferences;
    ProbabilityGenerator _pg;
};


class PackageSender{
public:
    //PackageSender(PackageSender&&);
    void send_package();
    std::optional<Package>& get_sending_buffer() {return bufor;}
    ReceiverPreferences receiver_preferences_;

protected:
    void push_package(Package&& pack) {bufor = std::move(pack);};

private:
    std::optional<Package> bufor;
};


class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di): _id(id), _di(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() {return _di;}
    ElementID get_id() const { return _id; }
private:
    ElementID _id;
    TimeOffset _di;
};


class Worker : public PackageSender, public IPackageReceiver{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : _id(id), _pd(pd), _q(std::move(q)), _t(0), working(std::nullopt) {}
    ElementID get_id() const { return _id; }
    void receive_package(Package&& pack) { _q->push(std::move(pack)); }
    void do_work(Time t);
    ReceiverType get_receiver_type() { return ReceiverType::WORKER; };

    TimeOffset get_processing_duration () {return _pd;}
    Time get_package_processing_start_time()  {return _t;}

    // iteratory
    const_it begin() const { return _q->cbegin(); };
    const_it end() const { return _q->cend(); };
    const_it cbegin() const { return _q->cbegin(); };
    const_it cend() const { return _q->cend(); };
private:
    ElementID _id;
    TimeOffset _pd;
    std::unique_ptr<IPackageQueue> _q;
    Time _t;
    std::optional<Package> working;
};

#endif //NETSIM_NODES_HPP
