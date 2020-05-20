cmake_minimum_required(VERSION 2.8)

project(pipes) 

find_package(jetson-utils)
find_package(jetson-inference)

find_package(CUDA)

#cuda_add_executable(pipes boilerplate.cpp)
add_executable(pipes boilerplate.cpp)

target_link_libraries(pipes jetson-inference)

