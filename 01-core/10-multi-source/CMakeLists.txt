cmake_minimum_required(VERSION 3.20)
project(Lab LANGUAGES CXX)

option(LAB_VERBOSE "Print the LAB diagnostic line" OFF)
add_subdirectory(app)
