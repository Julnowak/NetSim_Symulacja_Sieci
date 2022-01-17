//
// Created by Julia on 01.01.2022.
//

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "nodes.hpp"
#include "types.hpp"

#include <algorithm>
#include <exception>


enum NodeColor{
    UNVISITED, VISITED, VERIFIED
};

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);


template <typename Node>
class NodeCollection{
public:
    using container_t = typename std::vector<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node &node){Nodes.push_back(std::move(node));}
    void remove_by_id(ElementID _id);

    iterator find_by_id(ElementID _id) {auto it = std::find_if(Nodes.begin(), Nodes.end(),[_id](const auto& elem){ return (elem.get_id() == _id);}); return it;}
    const_iterator find_by_id(ElementID _id) const {auto it = std::find_if(Nodes.begin(), Nodes.end(),[_id](const auto& elem){return (elem.get_id() == _id);}); return it;}

    // iteratory
    iterator begin() {return Nodes.begin();}
    iterator end() {return Nodes.end();}
    const_iterator begin() const {return Nodes.begin();};
    const_iterator end() const {return Nodes.end();};
    const_iterator cbegin() const {return Nodes.cbegin();}
    const_iterator cend() const {return Nodes.cend();}


private:
    container_t Nodes;
};


template<typename Node>
void NodeCollection<Node>::remove_by_id(ElementID _id)
{
    auto it = find_by_id(_id);
    if(it != Nodes.end()) Nodes.erase(it);
}


class Factory{
public:
    bool is_consistent() const;
    void do_deliveries(Time time) {for (auto &ramp : Ramps) ramp.deliver_goods(time);}
    void do_package_passing();
    void do_work(Time time) {for(auto &worker:Workers) worker.do_work(time);}


    // RAMP
    void add_ramp(Ramp &&ramp){Ramps.add(ramp);}
    void remove_ramp(ElementID id);

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return Ramps.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return Ramps.find_by_id(id);}

    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return Ramps.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend() const {return Ramps.cend();}
    NodeCollection<Ramp>::const_iterator ramp_begin() const {return Ramps.begin();}
    NodeCollection<Ramp>::const_iterator ramp_end() const {return Ramps.end();}


    // WORKER
    void add_worker(Worker &&worker){Workers.add(worker);}
    void remove_worker(ElementID id);

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return Workers.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return Workers.find_by_id(id);}

    NodeCollection<Worker>::const_iterator worker_cbegin() const {return Workers.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend() const {return Workers.cend();}
    NodeCollection<Worker>::const_iterator worker_begin() const {return Workers.begin();}
    NodeCollection<Worker>::const_iterator worker_end() const {return Workers.end();}


    // STOREHOUSE
    void add_storehouse(Storehouse && storehouse){Storehouses.add(storehouse);}
    void remove_storehouse(ElementID id);

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return Storehouses.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return Storehouses.find_by_id(id);}

    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return Storehouses.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return Storehouses.cend();}
    NodeCollection<Storehouse>::const_iterator storehouse_begin() const {return Storehouses.begin();}
    NodeCollection<Storehouse>::const_iterator storehouse_end() const {return Storehouses.end();}
private:
    NodeCollection<Ramp> Ramps;
    NodeCollection<Worker> Workers;
    NodeCollection<Storehouse> Storehouses;

    template<typename Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
};


// IO_FACTORY_UPDATE
enum class ElementType{
    RAMP, WORKER, STOREHOUSE, LINK
};

struct ParsedLineData{
    ParsedLineData(ElementType et, std::map<std::string, std::string> map): element_type(et),  parameters (std::move(map)) {};

    ElementType element_type;
    std::map<std::string, std::string> parameters;
};


ParsedLineData parse_line (std::string line);

Factory load_factory_structure(std::istream& is);

void save_factory_structure (Factory& factory, std::ostream& os);


#endif //NETSIM_FACTORY_HPP
