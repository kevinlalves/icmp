cmake_minimum_required(VERSION 3.0.0)
project(icmp VERSION 0.1.0)
# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)


add_subdirectory(test)

add_executable(server server_main.cc icmp.cc tcp.cc)
add_executable(client client_main.cc icmp.cc tcp.cc)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
