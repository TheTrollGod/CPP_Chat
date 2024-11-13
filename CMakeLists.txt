cmake_minimum_required(VERSION 3.29)
project(CPP_Chat)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(inc)
include_directories(lib)
include_directories(src)

add_executable(CPP_Chat
    src/client.cpp
    src/common.h
    src/encryption.cpp
    src/encryption.h
    src/server.cpp)
