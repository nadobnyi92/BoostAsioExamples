project(SyncTcpClient)

set(SRC main.cpp
        client.cpp)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} pthread boost_program_options)
