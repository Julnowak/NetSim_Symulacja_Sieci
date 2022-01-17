//
// Created by Julia on 10.01.2022.
//

#ifndef NETSIM_SIMULATION_HPP
#define NETSIM_SIMULATION_HPP

#include "factory.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&,Time)>);

#endif //NETSIM_SIMULATION_HPP
