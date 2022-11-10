project(AsyncTcpServer)

set(SRC main.cpp
        async_server.cpp
        client_storage.cpp)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} pthread)
