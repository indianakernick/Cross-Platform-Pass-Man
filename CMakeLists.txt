cmake_minimum_required(VERSION 3.8)
project(Pass_Man_cmake)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        Sources/app.cpp
        Sources/app.hpp
        Sources/encrypt.cpp
        Sources/encrypt.hpp
        "Sources/generate key.cpp"
        "Sources/generate key.hpp"
        "Sources/generate password.cpp"
        "Sources/generate password.hpp"
        "Sources/interpret commands.cpp"
        "Sources/interpret commands.hpp"
        Sources/main.cpp
        Sources/parse.cpp
        Sources/parse.hpp
        Sources/passwords.hpp
        "Sources/write to clipboard.cpp"
        "Sources/write to clipboard.hpp")

add_executable(Pass_Man_cmake ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -Wpedantic -std=c++1z")