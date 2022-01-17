//
// Created by Julia on 10.01.2022.
//

#include "simulation.hpp"

void simulate(Factory &factory, TimeOffset d, std::function<void(Factory &, Time)> generate_report)
{
    for(TimeOffset time = 1; time <= d; time++)
    {
        factory.do_deliveries(time);

        factory.do_package_passing();

        factory.do_work(time);

        generate_report(factory,time);
    }
}