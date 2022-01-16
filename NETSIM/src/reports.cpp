//
// Created by Julia on 10.01.2022.
//

#include "reports.hpp"

void generate_structure_report(const Factory &f, std::ostream &os) {
    os.flush(); // Czyszczenie

    std::vector<unsigned  int> IDs;
    for(auto it = f.ramp_cbegin(); it != f.ramp_cend(); ++it) IDs.push_back(it->get_id());
    std::sort(IDs.begin(), IDs.end());

    os << std::endl << "== LOADING RAMPS ==" << std::endl << std::endl;


    for(auto &id : IDs)
    {
        auto ramp = f.find_ramp_by_id(id);

        os << "LOADING RAMP #" << ramp->get_id() << std::endl;
        os << "  Delivery interval: " << ramp->get_delivery_interval() << std::endl;
        os << "  Receivers:" << std::endl;

        std::vector<IPackageReceiver*> rec;
        for(auto& pair : ramp->receiver_preferences_.get_preferences())
            if(pair.first->get_receiver_type() == STOREHOUSE) rec.push_back(pair.first);

        std::sort(rec.begin(), rec.end(),[](auto& elem1, auto& elem2)
        {return elem1->get_id() < elem2->get_id();});

        for(auto& ipr : rec) os << "    storehouse #" << ipr->get_id() << std::endl;

        rec.clear();
        for(auto& pair : ramp->receiver_preferences_.get_preferences())
            if(pair.first->get_receiver_type() == WORKER) rec.push_back(pair.first);

        std::sort(rec.begin(), rec.end(),[](auto& elem1, auto& elem2)
        {return elem1->get_id() < elem2->get_id();});

        for(auto& ptr : rec) os << "    worker #" << ptr->get_id() << std::endl;

        os << std::endl;
    }


    IDs.clear();
    for(auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) IDs.push_back(it->get_id());
    std::sort(IDs.begin(), IDs.end());

    os << std::endl << "== WORKERS ==" << std::endl << std::endl;

    for(auto &id : IDs)
    {
        auto worker = f.find_worker_by_id(id);

        os << "WORKER #" << worker->get_id() << std::endl;
        os << "  Processing time: " << worker->get_processing_duration() << std::endl;

        if(worker->get_queue()->get_queue_type() == PackageQueueType::FIFO) os << "  Queue type: FIFO" << std::endl;
        else os << "  Queue type: LIFO" << std::endl;

        os << "  Receivers:" << std::endl;

        std::vector<IPackageReceiver*> rec;

        for(auto& pair : worker->receiver_preferences_.get_preferences())
            if(pair.first->get_receiver_type() == STOREHOUSE) rec.push_back(pair.first);

        std::sort(rec.begin(), rec.end(),[](auto& elem1, auto& elem2)
        {return elem1->get_id() < elem2->get_id();});

        for(auto& ptr : rec) os << "    storehouse #" << ptr->get_id() << std::endl;

        rec.clear();
        for(auto& pair : worker->receiver_preferences_.get_preferences())
            if(pair.first->get_receiver_type() == WORKER) rec.push_back(pair.first);

        std::sort(rec.begin(), rec.end(),[](auto& elem1, auto& elem2)
        {return elem1->get_id() < elem2->get_id();});

        for(auto& ptr : rec) os << "    worker #" << ptr->get_id() << std::endl;

        os << std::endl;
    }


    IDs.clear();
    for(auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); ++it) IDs.push_back(it->get_id());
    std::sort(IDs.begin(), IDs.end());

    os << std::endl << "== STOREHOUSES ==" << std::endl << std::endl;

    for(auto &id : IDs)
    {
        auto storehouse_it = f.find_storehouse_by_id(id);
        os << "STOREHOUSE #" << storehouse_it->get_id() << std::endl << std::endl;
    }
}


void generate_simulation_turn_report(const Factory &f, std::ostream &os, Time t) {
    os << "=== [ Turn: " << t << " ] ===" << std::endl;

    std::vector<unsigned  int> IDs;
    for(auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) IDs.push_back(it->get_id());
    std::sort(IDs.begin(), IDs.end());

    os << "\n== WORKERS ==\n";

    for(auto &id : IDs)
    {
        auto worker = f.find_worker_by_id(id);
        os << "\nWORKER #" << worker->get_id() << std::endl;

        if(worker->get_processing_buffer())
        {
            os << "  PBuffer: #" << worker->get_processing_buffer().value().get_id();
            os << " (pt = " << t - worker->get_package_processing_start_time() + 1 << ")" << std::endl;
        }
        else os << "  PBuffer: (empty)" << std::endl;

        if(worker->get_queue()->empty()) os << "  Queue: (empty)" << std::endl;
        else
        {
            os << "  Queue: ";
            unsigned int i = 0;
            for(auto& p : *(worker->get_queue()))
            {
                os << "#" << p.get_id();
                if (i != worker->get_queue()->size() - 1) os <<", ";
                i++;
            }
            os << std::endl;
        }

        os << "  SBuffer: ";
        if(worker->get_sending_buffer() != std::nullopt) os << "#" << worker->get_sending_buffer().value().get_id() << std::endl;
        else os << "(empty)" << std::endl;
    }


    IDs.clear();
    for(auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); ++it) IDs.push_back(it->get_id());
    std::sort(IDs.begin(), IDs.end());

    os << "\n\n== STOREHOUSES ==\n\n";

    for(auto &id : IDs)
    {
        auto storehouse = f.find_storehouse_by_id(id);

        os << "STOREHOUSE #" << storehouse->get_id() << std::endl;

        if(storehouse->cbegin() != storehouse->cend())
        {
            os << "  Stock:";
            for(auto& it : *storehouse) os << " #" << it.get_id();
            os << std::endl;
        }
        else os << "  Stock: (empty)" << std::endl;
    }

    os << std::endl;
}
