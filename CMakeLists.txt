cmake_minimum_required(VERSION 3.5)

project(BoostAsioExamples)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED system)

add_subdirectory(EchoSyncTcp/client)
add_subdirectory(EchoSyncTcp/server)

add_subdirectory(EchoAsyncTcp/client)
add_subdirectory(EchoAsyncTcp/server)

add_subdirectory(EchoSyncUdp/client)
add_subdirectory(EchoSyncUdp/server)

add_subdirectory(SimpleSyncTcp/client)
add_subdirectory(SimpleSyncTcp/server)

file(CREATE_LINK ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json SYMBOLIC)

