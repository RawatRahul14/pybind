cmake_minimum_required(VERSION 3.15)
project(hello)

find_package(pybind11 REQUIRED)

pybind11_add_module(speed_test hello.cpp)