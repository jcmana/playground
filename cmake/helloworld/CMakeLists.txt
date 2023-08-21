cmake_minimum_required(VERSION 2.8.9)

project (helloworld)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(helloworld source/helloworld.cpp)
