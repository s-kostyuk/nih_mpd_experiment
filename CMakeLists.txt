cmake_minimum_required(VERSION 2.8)

project(nih_mpd_experiment)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

set(SOURCES "main.cpp" "socket.h" "socket.cpp" "mpd_player.h" "mpd_player.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
