cmake_minimum_required(VERSION 3.21)
project(Project_0)

set(CMAKE_CXX_STANDARD 14)

include_directories(cs561_templatezonemaps)

add_executable(Project_0
        cs561_templatezonemaps/main.cpp
        cs561_templatezonemaps/workload_generator.cpp
        cs561_templatezonemaps/zonemaps.cpp
        cs561_templatezonemaps/zonemaps.h)
