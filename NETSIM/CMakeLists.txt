cmake_minimum_required(VERSION 3.21)
project(NETSIM)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)
add_executable(NETSIM main.cpp package.cpp package.hpp storage_types.hpp.cpp storage_types.hpp.hpp types.hpp)
