cmake_minimum_required(VERSION 3.20)
project(Lab LANGUAGES CXX)

option(LAB_VERBOSE "Print the LAB diagnostic line" OFF)
message(STATUS "LAB_VERBOSE=${LAB_VERBOSE}")

add_executable(lab_probe main.cpp)
if(LAB_VERBOSE)
    target_compile_definitions(lab_probe PRIVATE LAB_VERBOSE)
endif()
