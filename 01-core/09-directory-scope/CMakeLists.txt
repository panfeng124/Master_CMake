cmake_minimum_required(VERSION 3.20)
project(Lab LANGUAGES CXX)

option(LAB_VERBOSE "Print the LAB diagnostic line" OFF)
message(STATUS "parent before: ${LAB_VERBOSE}")
add_subdirectory(app)
message(STATUS "parent after: ${LAB_VERBOSE}")
