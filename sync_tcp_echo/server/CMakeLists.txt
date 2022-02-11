project(SyncTcpEchoServer)

set(SRC main.cpp)

add_executable(${PROJECT_NAME} ${SRC})

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} pthread)
