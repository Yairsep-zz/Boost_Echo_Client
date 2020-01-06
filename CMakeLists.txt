cmake_minimum_required(VERSION 3.15)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)
include_directories(src)

add_executable(Boost_Echo_Client
        .idea/.gitignore
        .idea/Boost_Echo_Client.iml
        .idea/misc.xml
        .idea/modules.xml
        .idea/vcs.xml
        .idea/workspace.xml
        cmake-build-debug/Boost_Echo_Client.cbp
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/Makefile
        include/Book.h
        include/Client.h
        include/connectionHandler.h
        include/Frame.h
        include/User.h
        src/Book.cpp
        src/Client.cpp
        src/connectionHandler.cpp
        src/echoClient.cpp
        src/Frame.cpp
        src/User.cpp
        CMakeLists.txt
        makefile)
