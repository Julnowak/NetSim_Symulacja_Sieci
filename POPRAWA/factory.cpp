//
// Created by Julia on 01.01.2022.
//


#include <sstream>
#include "factory.hpp"
#include "nodes.hpp"

bool Factory::is_consistent() const{
    std::map<const PackageSender *, NodeColor> color;
    for(auto &ramp : Ramps) color[&ramp] = UNVISITED;
    for(auto &worker : Workers) color[&worker] = UNVISITED;
    try
    {
        for(auto &ramp : Ramps) has_reachable_storehouse(&ramp, color);
    }
    catch(std::logic_error&){return false;}

    return true;
}


template<typename Node>
void Factory::remove_receiver(NodeCollection<Node> &collection, ElementID id) {
    collection.remove_by_id(id);
    if(std::is_same<Node, Worker>::value){
        for(auto &ramp : Ramps) ramp.receiver_preferences_.remove_receiver(&(*find_worker_by_id(id)));
        for(auto &worker : Workers) worker.receiver_preferences_.remove_receiver(&(*find_worker_by_id(id)));
    }
    if(std::is_same<Node, Storehouse>::value){
        for(auto &ramp : Ramps) ramp.receiver_preferences_.remove_receiver(&(*find_storehouse_by_id(id)));
        for(auto &worker : Workers) worker.receiver_preferences_.remove_receiver(&(*find_storehouse_by_id(id)));
    }
}


void Factory::do_package_passing()
{
    for(auto &ramp : Ramps) ramp.send_package();
    for(auto &worker : Workers) worker.send_package();
}


void Factory::remove_worker(ElementID id)
{
    remove_receiver(Workers, id);
}


void Factory::remove_storehouse(ElementID id)
{
    remove_receiver(Storehouses, id);
}


void Factory::remove_ramp(ElementID id)
{
    Ramps.remove_by_id(id);
}


bool has_reachable_storehouse(const PackageSender *sender, std::map<const PackageSender *, NodeColor> &node_colors) {
    if(node_colors[sender] == VERIFIED) return true;

    node_colors[sender] = VISITED;

    if(sender->receiver_preferences_.get_preferences().empty()) throw std::logic_error("There are no Receivers");

    bool has_at_least_one_except_self = false;

    for(auto &receiver_pair : sender->receiver_preferences_.get_preferences())
    {
        if(receiver_pair.first->get_receiver_type() == STOREHOUSE) has_at_least_one_except_self = true;
        else if (receiver_pair.first->get_receiver_type() == WORKER)
        {
            IPackageReceiver* receiver_ptr = receiver_pair.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrcv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if(sendrcv_ptr == sender) continue;

            has_at_least_one_except_self = true;

            if(node_colors[sendrcv_ptr] == UNVISITED) has_reachable_storehouse(sendrcv_ptr, node_colors);
        }
    }
    node_colors[sender] = VERIFIED;

    if(has_at_least_one_except_self) return true;
    else throw std::logic_error("Unreachable Storehouse");
}
