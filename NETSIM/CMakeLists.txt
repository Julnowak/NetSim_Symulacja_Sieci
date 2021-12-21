cmake_minimum_required(VERSION 3.21)

# Ustaw nazwę projektu.
project(NETSIM)

# Ustaw standard C++ (tu: C++17).
set(CMAKE_CXX_STANDARD 17)

# Dodaj flagi kompilacji wymuszające pisanie kodu zgodnego ze standardem.
add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

# Dodaj katalogi z plikami nagłówkowymi dla wszystkich konfiguracji.
include_directories(
        include
)

# Ustaw zmienną `SOURCE_FILES`, która będzie przechowywać ścieżki do
# tych plików źródłowych, które będą niezbędne w każdej konfiguracji.
set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        )

# Dodaj konfigurację typu `Debug`.
add_executable(netsim_tests_debug ${SOURCE_FILES} main.cpp)

