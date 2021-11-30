cmake_minimum_required(VERSION 3.0.0)
project(icmp VERSION 0.1.0)
# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(CTest)
enable_testing()

add_executable(client_test client_test.cc)
target_link_libraries(client_test gtest_main)
add_executable(server_test server_test.cc)
target_link_libraries(server_test gtest_main)
add_executable(icmp_test icmp_test.cc)
target_link_libraries(icmp_test gtest_main)

include(GoogleTest)
gtest_discover_tests(client_test)
gtest_discover_tests(server_test)
gtest_discover_tests(icmp_test)
