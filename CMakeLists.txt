cmake_minimum_required(VERSION 3.28)
project(CPP_Chat)

set(CMAKE_CXX_STANDARD 14)

# Include directories for common headers
include_directories(.)
include_directories(inc)
include_directories(lib)
include_directories(src)

# Server executable
add_executable(ServerAppLinux
        src/server.cpp
        src/common.h
        src/encryption.cpp
        src/encryption.h
)

# Client executable Linux
add_executable(LinuxClient
        src/client.cpp
        src/common.h
        src/encryption.cpp
        src/encryption.h
)

# Client executable Windows
add_executable(WindowsClient
        src/client-windows.cpp
        src/common.h
        src/encryption.h
        src/encryption.cpp
)

# Conditionally link the WinSock library for Windows
if(WIN32)
    # Add /lib as a library directory
    link_directories(${CMAKE_SOURCE_DIR}/lib)

    # Link necessary libraries for WindowsClient
    target_link_libraries(WindowsClient ws2_32 stdc++)
endif()

