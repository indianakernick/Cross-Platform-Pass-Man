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

add_subdirectory(dependencies/clip/)
include_directories(../dependencies/clip/)
target_link_libraries(passman clip)

if(APPLE AND UNIX)
  set(INSTALL_PATH "/usr/local/bin/")
elseif(WIN32)
  set(INSTALL_PATH "C:\\Program files (x86)")
else()
  message(FATAL_ERROR "Don't know where to install executable")
endif()

if(INSTALL_PATH)
  set_target_properties(
          passman PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY_DEBUG "${INSTALL_PATH}"
          RUNTIME_OUTPUT_DIRECTORY_RELEASE "${INSTALL_PATH}"
  )
endif()