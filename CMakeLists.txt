cmake_minimum_required(VERSION 3.20)
project(cpp_single_linked_list)

set(CMAKE_CXX_STANDARD 14)

include_directories(single-linked-list)

add_executable(cpp_single_linked_list
        single-linked-list/main.cpp
        single-linked-list/single-linked-list.h)
