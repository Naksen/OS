cmake_minimum_required(VERSION 3.16)
project(4_lab_c__)


set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(4_lab_child_1 child_1.cpp)
add_executable(4_lab_child_2 child_2.cpp)
add_executable(4_lab main.cpp)

target_link_libraries(4_lab PRIVATE Threads::Threads rt)
target_link_libraries(4_lab_child_1 PRIVATE Threads::Threads rt)
target_link_libraries(4_lab_child_2 PRIVATE Threads::Threads rt)

add_dependencies(4_lab 4_lab_child_1)
add_dependencies(4_lab_child_1 4_lab_child_2)
