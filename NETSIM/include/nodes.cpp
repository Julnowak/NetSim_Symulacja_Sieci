//
// Created by Julia on 01.01.2022.
//

#include "nodes.hpp"
#include "helpers.hpp"


void Ramp::deliver_goods(Time t)
{
    if (t % _di == 1)
    {
        Package pack;
        push_package(std::move(pack));
    }
}


void Worker::do_work(Time t) {
    if (!_processing_buffer)
    {
        if (!_q->empty())
        {
            _processing_buffer.emplace(_q->pop());
            _t = t;
        }
    }

    if (_processing_buffer)
    {
        if ((t - _t) == _pd - 1)
        {
            push_package(std::move(_processing_buffer.value()));
            _processing_buffer.reset();
        }
    }
}


void PackageSender::send_package()
{
    if (bufor)
    {
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(*bufor));
        bufor.reset();
    }
}


void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    double preference = 1;
    preferences[r] = preference;
    for(auto &i : preferences) {i.second = 1;}
    for(auto &i : preferences){ i.second /= static_cast<double>(preferences.size());}
}


IPackageReceiver *ReceiverPreferences::choose_receiver ()
{
    double p = _pg();
    double distributor_value = 0.0;
    for (auto& item: preferences) {
        distributor_value = distributor_value + item.second;
        if (p<=distributor_value) {
            return item.first;
        }
    }
    throw std::runtime_error("Error has occurred in ReceiverPreferences::choose_receiver()");
}


void ReceiverPreferences::remove_receiver(IPackageReceiver* r){
    if(preferences.find(r) != preferences.end())
    {
        preferences.erase(r);
        if (preferences.empty()) return;
        for(auto &i : preferences) {i.second = 1;}
        for(auto &i : preferences){ i.second /= static_cast<double>(preferences.size());}
    }
}
