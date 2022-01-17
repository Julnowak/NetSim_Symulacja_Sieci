//
// Created by Julia on 10.01.2022.
//

#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP

#include "factory.hpp"

// Raportowanie

void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);


// Symulacja

class IntervalReportNotifier{
public:
    IntervalReportNotifier(TimeOffset to): _to(to){};
    bool should_generate_report(Time t) const
    {
        if(_to == 1) return true;
        else return t % _to == 1;
    }

private:
    TimeOffset _to;
};


class SpecificTurnsReportNotifier{
public:
    SpecificTurnsReportNotifier(std::set<Time> turns): _turns(std::move(turns)) {};
    bool should_generate_report(Time t) const {return _turns.find(t) != _turns.end();}

private:
    std::set<Time> _turns;
};

#endif //NETSIM_REPORTS_HPP
