//
// Created by Julia on 01.01.2022.
//


#include "factory.hpp"

struct no_reachable_storehouse_error : public std::exception
{
    const char * what () const throw (){return "No reachable storehouse";}
};


bool Factory::is_consistent()
{
    if(Ramps.begin()==Ramps.end()) return false;
    if(Workers.begin()==Workers.end()) return false;
    if(Storehouses.begin()==Storehouses.end()) return false;

    bool consistent = true;

    std::for_each
    (Ramps.begin(),Ramps.end(),[&consistent](Ramp &ramp)
    {
        auto preferences=ramp.receiver_preferences_.get_preferences();
        if(preferences.empty()) consistent=false;
    });


    std::for_each
    (Workers.begin(),Workers.end(),[this,&consistent](Worker &worker)
    {
        auto id=worker.get_id();
        bool no_empty = false;

        std::for_each
        (Ramps.begin(),Ramps.end(),[id,&no_empty](Ramp &ramp)
        {
            auto preferences= ramp.receiver_preferences_.get_preferences();
            for(auto & preference : preferences)
                if(preference.first->get_id()==id) no_empty = true;
        });

        std::for_each
        (Workers.begin(),Workers.end(),[id,&no_empty](Worker &worker)
        {
            if(worker.get_id() != id)
            {
                auto preferences = worker.receiver_preferences_.get_preferences();
                for (auto & preference : preferences)
                    if (preference.first->get_id() == id) no_empty = true;
            }
        });

        auto workerPreferences = worker.receiver_preferences_.get_preferences();
        auto workerId = worker.get_id();
        bool check = false;

        for(auto & workerPreference : workerPreferences)
        {
            auto type=workerPreference.first->get_receiver_type();
            if(workerPreference.first->get_id()!=workerId || type==STOREHOUSE) check = true;
        }

        if(!no_empty || !check) consistent = false;

    });


    std::for_each
    (Storehouses.begin(),Storehouses.end(),[this,&consistent](Storehouse &storehouse)
    {
        auto id= storehouse.get_id();
        bool no_empty = false;

        std::for_each
        (Workers.begin(),Workers.end(),[id,&no_empty](Worker &worker)
        {
            auto preferences=worker.receiver_preferences_.get_preferences();
            for(auto & preference : preferences)
                if(preference.first->get_id()==id) no_empty = true;
        });

        if(!no_empty) consistent = false;
    });

    return consistent;
}


void Factory::do_package_passing()
{
    std::for_each(Ramps.begin(),Ramps.end(),[](Ramp &ramp){ ramp.send_package();});
    std::for_each(Workers.begin(),Workers.end(),[](Worker &worker){ worker.send_package();});
}


void Factory::remove_worker(ElementID id)
{
    std::for_each
    (Ramps.begin(),Ramps.end(),[id](Ramp &ramp)
    {
        auto preferences=ramp.receiver_preferences_.get_preferences();
        for(auto & preference : preferences)
            if(preference.first->get_id()==id) ramp.receiver_preferences_.remove_receiver(preference.first);
    });

    Workers.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id)
{
    std::for_each
    (Workers.begin(),Workers.end(),[id](Worker &worker)
    {
        auto preferences=worker.receiver_preferences_.get_preferences();
            for(auto & preference : preferences)
                if(preference.first->get_id()==id) worker.receiver_preferences_.remove_receiver(preference.first);
    });

    Storehouses.remove_by_id(id);
}
