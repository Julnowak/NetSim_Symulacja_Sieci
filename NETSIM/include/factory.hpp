//
// Created by Julia on 01.01.2022.
//

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "nodes.hpp"
#include "types.hpp"

#include <algorithm>
#include <exception>

template <typename Node>
class NodeCollection{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node &node){Nodes.push_back(std::move(node));}
    void remove_by_id(ElementID _id);
    iterator find_by_id(ElementID _id) {auto it = std::find_if(Nodes.begin(), Nodes.end(),[_id](const auto& elem){ return (elem.get_id() == _id);}); return it;}
    const_iterator find_by_id(ElementID _id) const {auto it = std::find_if(Nodes.begin(), Nodes.end(),[_id](const auto& elem){return (elem.get_id() == _id);}); return it;}

    // iteratory
    iterator begin() {return Nodes.begin();}
    iterator end() {return Nodes.end();}
    const_iterator cbegin(){return Nodes.cbegin();}
    const_iterator cend(){return Nodes.cend();}

private:
    container_t Nodes;
};


template<typename Node>
void NodeCollection<Node>::remove_by_id(ElementID _id)
{
    NodeCollection::iterator it = find_by_id(_id);
    if(it != Nodes.end()) Nodes.erase(it);
}


class Factory{
public:
    bool is_consistent();
    void do_deliveries(Time time) {std::for_each(Ramps.begin(),Ramps.end(),[time](Ramp& ramp){ramp.deliver_goods(time);});}
    void do_package_passing();
    void do_work(Time time) {std::for_each(Workers.begin(),Workers.end(),[time](Worker& worker){worker.do_work(time);});}

    // RAMP
    void add_ramp(Ramp&& node){Ramps.add(node);}
    void remove_ramp(ElementID id){Ramps.remove_by_id(id);}

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return Ramps.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return Ramps.find_by_id(id);}

    NodeCollection<Ramp>::const_iterator ramp_cbegin() {return Ramps.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend() {return Ramps.cend();}


    // WORKER
    void add_worker(Worker&& node){Workers.add(node);}
    void remove_worker(ElementID id);

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return Workers.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return Workers.find_by_id(id);}

    NodeCollection<Worker>::const_iterator worker_cbegin() {return Workers.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend() {return Workers.cend();}


    // STOREHOUSE
    void add_storehouse(Storehouse&& node){Storehouses.add(node);}
    void remove_storehouse(ElementID id);

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return Storehouses.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return Storehouses.find_by_id(id);}

    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() {return Storehouses.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend() {return Storehouses.cend();}

private:
    NodeCollection<Ramp> Ramps;
    NodeCollection<Worker> Workers;
    NodeCollection<Storehouse> Storehouses;
};


#endif //NETSIM_FACTORY_HPP
