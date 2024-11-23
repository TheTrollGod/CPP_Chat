cmake_minimum_required(VERSION 3.29)
project(CPP_Chat)

set(CMAKE_CXX_STANDARD 14)

# Include directories for common headers
include_directories(.)
include_directories(inc)
include_directories(lib)
include_directories(src)

# Server executable
add_executable(ServerApp
        src/server.cpp
        src/common.h
        src/encryption.cpp
        src/encryption.h
)

# Client executable
add_executable(ClientApp
        src/client.cpp
        src/common.h
        src/encryption.cpp
        src/encryption.h
)