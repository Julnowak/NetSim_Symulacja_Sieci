//
// Created by Julia on 19.12.2021.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include "types.hpp"

class Package{
    Package();
    explicit Package(ElementID);
    Package(Package&&) noexcept ;
    [[nodiscard]] ElementID get_id() const{ return _id; }
    Package& operator = (Package&&) noexcept ;
    ~Package();
};

#endif //NETSIM_PACKAGE_HPP
