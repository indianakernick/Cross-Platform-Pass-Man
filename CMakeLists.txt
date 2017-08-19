cmake_minimum_required(VERSION 3.8)
project(Pass_Man_cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -Wpedantic -std=c++1z")

set(SOURCE_FILES
        Sources/app.cpp
        Sources/app.hpp
        Sources/encrypt.cpp
        Sources/encrypt.hpp
        "Sources/interpret commands.cpp"
        "Sources/interpret commands.hpp"
        Sources/main.cpp
        Sources/parse.cpp
        Sources/parse.hpp
        "Sources/write to clipboard.cpp"
        "Sources/write to clipboard.hpp")

add_executable(passman ${SOURCE_FILES})

include_directories(/usr/local/Cellar/sdl2/2.0.5/include)
target_link_libraries(passman /usr/local/Cellar/sdl2/2.0.5/lib/libSDL2.dylib)