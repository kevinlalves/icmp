cmake_minimum_required(VERSION 3.0.0)
project(icmp VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(server server_main.cc icmp.cc ping.cc)
add_executable(client client_main.cc icmp.cc ping.cc)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
