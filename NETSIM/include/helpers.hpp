//
// Created by Julia on 01.01.2022.
//

#ifndef NETSIM_HELPERS_HPP
#define NETSIM_HELPERS_HPP

#include <functional>
#include <random>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

#endif //NETSIM_HELPERS_HPP
