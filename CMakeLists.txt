cmake_minimum_required(VERSION 3.5)

project(BoostAsioExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED system)

add_subdirectory(sync_tcp_echo)
