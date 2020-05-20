cmake_minimum_required(VERSION 2.8)

project(pipes) 

find_package(jetson-utils)

#cuda_add_executable(pipes pipes.cpp)
add_executable(pipes boilerplate.cpp)

target_link_libraries(pipes jetson-inference)
