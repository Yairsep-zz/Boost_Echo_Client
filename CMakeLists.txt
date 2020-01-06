cmake_minimum_required(VERSION 3.15)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(bin)
include_directories(include)
include_directories(src)

add_executable(Boost_Echo_Client
        include/connectionHandler.h
        src/connectionHandler.cpp
        src/echoClient.cpp)
