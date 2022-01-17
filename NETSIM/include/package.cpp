//
// Created by Julia on 19.12.2021.
//

#include "package.hpp"
#include <stdexcept>


Package::Package()
{
    if (freed_IDs.empty())
        _id = (*assigned_IDs.rbegin()) + 1;
    else
    {
        _id = *freed_IDs.begin();
        freed_IDs.erase(_id);
    }
    assigned_IDs.insert(_id);
}


Package::Package(ElementID id)
{
    if (assigned_IDs.find(id) != assigned_IDs.end())
        throw std::invalid_argument("ID is already assigned");

    _id = id;
    assigned_IDs.insert(_id);

    // Erase if exist
    if (freed_IDs.find(id) != freed_IDs.end()) freed_IDs.erase(id);
}


Package::Package(Package&& other) noexcept{
    _id = other._id;
    other._id = 0;
}


Package& Package::operator=(Package&& other) noexcept {

    if (_id != 0)
    {
        freed_IDs.insert(_id);
        assigned_IDs.erase(_id);
    }

    _id = other._id;
    other._id = 0;

    return *this;
}


Package::~Package()
{
    if (_id != 0)
    {
        // if not exist - insert
        if (freed_IDs.find(_id) == freed_IDs.end()) freed_IDs.insert(_id);

        // Erase if exist
        if (assigned_IDs.find(_id) != assigned_IDs.end()) assigned_IDs.erase(_id);
    }
}