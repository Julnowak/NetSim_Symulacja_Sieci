//
// Created by Julia on 19.12.2021.
//

#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <functional>

using ElementID = unsigned int;     // Używamy, gdyż liczby nie są ujemne
using size_type = unsigned int;     // typ pomocniczy, określający  liczbę półproduktów w kontenerze

using Time = unsigned int;     // Używamy, gdyż liczby nie są ujemne
using TimeOffset = unsigned int;     // Używamy, gdyż liczby nie są ujemne
using ProbabilityGenerator = std::function<double()>;

#endif //NETSIM_TYPES_HPP
