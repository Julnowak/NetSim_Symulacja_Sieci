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

    if (!working && !_q->empty())
    {
        working = _q->pop();
        _t = t;
    }

    if (working && t - _t == _pd - 1)
    {
        push_package(std::move(*working));
        working.reset();
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


//bool PackageSender::buffer_empty(){
//    if (bufor) return true;
 //   else return false;
//}

void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    double preference = 1.0;
    double suma = 0.0;
    preferences[r] = preference;
    for (auto& pair : preferences)
    {
        preferences[pair.first] = 1.0 / static_cast<double>(preferences.size());
        suma = suma + pair.second;
    }
    if (suma != 1.0) preferences[r] = preferences[r] + 1.0 - suma;

}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r){
    IPackageReceiver* copy_r = r;
    double suma = 0.0;
    preferences.erase(r);

    if(!preferences.empty())
        {
            for (const auto &pair : preferences)
            {
                preferences[pair.first]= 1.0 / static_cast<double>(preferences.size());
                suma = suma + pair.second;
                copy_r  = pair.first;
            }

            if (suma!= 1.0) preferences[copy_r ] = preferences[copy_r ] + 1.0 - suma;
        }
}


IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    double los = default_probability_generator();
    double dis = 0.0;
    IPackageReceiver* helper = preferences.rbegin()->first;

    std::map<IPackageReceiver*, double>::reverse_iterator it;

    for (it = preferences.rbegin(); it != preferences.rend(); ++it)
    {
        dis = dis + it->second;
        helper = it->first;
        if (los < dis) break;
    }

    return helper;
}