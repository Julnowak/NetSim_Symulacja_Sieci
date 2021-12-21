//
// Created by Julia on 19.12.2021.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include "types.hpp"
#include <set>

class Package {
public:
    Package();
    explicit Package(ElementID);
    Package(Package&&) noexcept;
    Package& operator = (Package&&) noexcept;
    ElementID get_id() const {return _id;}
    ~Package();

private:
    ElementID _id = 0;
    inline static std::set<ElementID> assigned_IDs{0};
    inline static std::set<ElementID> freed_IDs;
};


#endif //NETSIM_PACKAGE_HPP
