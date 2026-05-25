cmake_minimum_required(VERSION 3.15)
project(socpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(socpp src/socket.cpp)
target_include_directories(socpp PUBLIC include)

enable_testing()
add_executable(test_lifecycle tests/test_lifecycle.cpp)
add_executable(test_connect tests/test_connect.cpp)
target_link_libraries(test_lifecycle PRIVATE socpp)
target_link_libraries(test_connect PRIVATE socpp)
add_test(NAME lifecycle COMMAND test_lifecycle)
add_test(NAME connect COMMAND test_connect)
