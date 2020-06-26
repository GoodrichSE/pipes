cmake_minimum_required(VERSION 2.8)

project(pipes) 

# link to linux libraries, SDK libraries, packages
include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include/usr/local/lib/ /usr/local/include/jetson-utils /usr/local/cuda-10.0/targets/aarch64-linux/include/ /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)

#message("Search path: ${CMAKE_LIBRARY_PATH}")
#message("Prefix path: ${CMAKE_PREFIX_PATH}")

find_package(jetson-utils)
find_package(jetson-inference)

#message("Found jetson-utils? ${jetson-utils_FOUND}")
#message("Found jetson-inference? ${jetson-inference_FOUND}")

find_package(CUDA)

#message("Found CUDA? ${CUDA_FOUND}")

# link to jetson libraries
#include_directories(/usr/local/include/jetson-utils /usr/local/lib)

add_library(
	test-lib SHARED
	TestClass.h
	TestClass.cpp
	)

#cuda_add_executable(pipes boilerplate.cpp)
add_executable(pipes pipes.cpp)

target_link_libraries(pipes jetson-inference jetson-utils test-lib)
#target_link_libraries(pipes jetson-inference jetson-utils/commandLine.h)

