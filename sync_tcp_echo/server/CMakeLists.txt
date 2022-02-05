project(SyncTcpEchoServer)

set(SRC main.cpp)

add_executable(${PROJECT_NAME} ${SRC})

include_directories(${Boost_INCLUDE_DIRS})
