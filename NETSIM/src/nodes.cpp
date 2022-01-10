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


void Worker::do_work(Time t){

    if (working && t - _t == _pd - 1)
    {
        push_package(std::move(*working));
        working.reset();
    }

    if (!working && !_q->empty())
    {
        working = _q->pop();
        _t = t;
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


IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    double los = default_probability_generator();
    double suma = 0;
    IPackageReceiver* helper = preferences.rbegin()->first;

    std::map<IPackageReceiver*, double>::reverse_iterator it;

    for (it = preferences.rbegin(); it != preferences.rend(); ++it)
    {
        suma = suma + it->second;
        helper = it->first;
        if (los < suma) break;
    }

    return helper;
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
