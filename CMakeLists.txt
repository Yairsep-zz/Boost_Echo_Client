cmake_minimum_required(VERSION 3.15)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(bin)
include_directories(include)
include_directories(src)
link_libraries("-lboost_system -pthread")

add_executable(Boost_Echo_Client
        include/connectionHandler.h
        src/echoClient.cpp
        src/connectionHandler.cpp
        src/Client.cpp include/Client.h src/Book.cpp include/Book.h src/User.cpp include/User.h)


find_package(Boost 1.65.1 REQUIRED COMPONENTS
        date_time filesystem iostreams system)
find_package(Threads)
#target_link_libraries(Client Boost::date_time Boost::filesystem
#        Boost::iostreams Boost::system)