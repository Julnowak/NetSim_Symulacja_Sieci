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

Factory load_factory_structure(std::istream& is) {
    Factory factory;
    std::string line;

    while (std::getline(is,line))
    {

        bool is_blank_line = line.empty() || isblank(line[0]);
        bool is_comment_line = !line.empty() && line[0] == ';';

        if(is_blank_line || is_comment_line) continue;

        ParsedLineData plin = parse_line(line);

        if (plin.element_type == ElementType::RAMP)
        {
            int id = std::stoi(plin.parameters["id"]);
            int delivery_interval = std::stoi(plin.parameters["delivery-interval"]);

            factory.add_ramp(Ramp(id,delivery_interval));
        }

        if (plin.element_type == ElementType::WORKER)
        {
            PackageQueueType que_t;

            int id = std::stoi(plin.parameters["id"]);
            int processing_time = std::stoi(plin.parameters["processing-time"]);

            if (plin.parameters["queue-type"] == "LIFO") que_t = PackageQueueType::LIFO;
            if (plin.parameters["queue-type"] == "FIFO") que_t = PackageQueueType::FIFO;

            factory.add_worker(Worker(id,processing_time,std::make_unique<PackageQueue>(que_t)));
        }

        if (plin.element_type == ElementType::STOREHOUSE)
        {
            int id = std::stoi(plin.parameters["id"]);

            factory.add_storehouse(id);
        }

        if (plin.element_type == ElementType::LINK)
        {
            std::string::size_type conSRC = plin.parameters["src"].find('-');
            std::string typeSRC = plin.parameters["src"].substr(0,conSRC);
            int srcID = std::stoi(plin.parameters["src"].substr(conSRC+1));

            std::string::size_type conEND = plin.parameters["dest"].find('-');
            std::string typeEND = plin.parameters["dest"].substr(0,conEND);
            int endID = std::stoi(plin.parameters["dest"].substr(conEND+1));

            if (typeSRC == "worker")
            {
                if (typeEND == "worker") factory.find_worker_by_id(srcID)->receiver_preferences_.add_receiver(&(*(factory.find_worker_by_id(endID))));
                if (typeEND == "store") factory.find_worker_by_id(srcID)->receiver_preferences_.add_receiver(&(*(factory.find_storehouse_by_id(endID))));
            }

            if (typeSRC == "ramp")
            {
                if (typeEND == "worker") (factory.find_ramp_by_id(srcID))->receiver_preferences_.add_receiver(&(*(factory.find_worker_by_id(endID)))); //gwiazdka na początku
                if (typeEND == "store")(factory.find_ramp_by_id(srcID))->receiver_preferences_.add_receiver(&(*(factory.find_storehouse_by_id(endID))));
            }
        }
    }
    return factory;
}


ParsedLineData parse_line(std::string line) {
    // Ze strony
    std::vector<std::string> tokens {};
    std::string token;
    std::istringstream token_stream(line);
    char delimiter = ' ';
    while(std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }
    //

    std::map<std::string,std::string> mapID = {};
    for (std::size_t i = 1; i<tokens.size();i++)
    {
        std::string::size_type pos = tokens[i].find('=');
        mapID[tokens[i].substr(0,pos)] = tokens[i].substr(pos+1);
    }

    ElementType et;
    if (tokens[0] == "LOADING_RAMP") et = ElementType::RAMP;
    else if (tokens[0] == "WORKER") et = ElementType::WORKER;
    else if (tokens[0] == "STOREHOUSE") et = ElementType::STOREHOUSE;
    else if (tokens[0] == "LINK") et = ElementType::LINK;
    else throw std::logic_error("Somthin' wrong with tokens");

    return ParsedLineData(et,mapID);
}


void save_factory_structure (Factory& factory, std::ostream& os) {
    os.flush(); // Czyszczenie

    os << "; == LOADING RAMPS ==\n\n";

    std::for_each(factory.ramp_cbegin(),factory.ramp_cend(), [&os](auto& ramp)
    {os<<"LOADING_RAMP ""id="<<ramp.get_id() <<" delivery-interval="<<ramp.get_delivery_interval()<<"\n";});

    os <<"\n; == WORKERS ==\n\n";

    std::for_each(factory.worker_cbegin(),factory.worker_cend(), [&os](auto& worker)
    {
        os<<"WORKER ""id="<<worker.get_id() <<" processing-time="<<worker.get_processing_duration()<<" queue-type=";
        if (worker.get_queue()->get_queue_type() == PackageQueueType::FIFO) os<<"FIFO\n";
        if (worker.get_queue()->get_queue_type() == PackageQueueType::LIFO) os<<"LIFO\n";
    });

    os <<"\n; == STOREHOUSES ==\n\n";

    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&os](auto& storehouse)
    {os<<"STOREHOUSE id="<<storehouse.get_id()<<"\n";});

        os <<"\n; == LINKS ==\n\n";

        std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](auto& ramp)
        {
            std::for_each(ramp.receiver_preferences_.cbegin(), ramp.receiver_preferences_.cend(), [&os, &ramp](auto& item)
            {
                os << "LINK src=ramp-" << ramp.get_id() << " dest=";
                if(item.first->get_receiver_type() == ReceiverType::STOREHOUSE) os<<"store-"<<item.first->get_id()<<"\n";
                if(item.first->get_receiver_type() == ReceiverType::WORKER) os<<"worker-"<<item.first->get_id()<<"\n";
            });
        });

        std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](auto& worker)
        {
            std::for_each(worker.receiver_preferences_.cbegin(), worker.receiver_preferences_.cend(), [&os, &worker](auto& item)
            {
                os << "LINK src=worker-" << worker.get_id() << " dest=";
                if(item.first->get_receiver_type() == ReceiverType::STOREHOUSE) os<<"store-"<<item.first->get_id()<<"\n";
                if(item.first->get_receiver_type() == ReceiverType::WORKER) os<<"worker-"<<item.first->get_id()<<"\n";
            });
    });
}
