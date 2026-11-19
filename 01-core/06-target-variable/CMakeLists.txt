cmake_minimum_required(VERSION 3.20)
project(Lab LANGUAGES CXX)
set(app lab_cli_pf)

message(STATUS "value of variable app is ${app}" )


add_executable(lab_cli main.cpp)
add_executable(${app} main.cpp)