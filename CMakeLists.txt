cmake_minimum_required(VERSION 3.14)
project(optimization_methods CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(optimization_methods
    main.cpp
    src/UniformSearchMinimizer.cpp
    src/GoldenSectionMinimizer.cpp
)
