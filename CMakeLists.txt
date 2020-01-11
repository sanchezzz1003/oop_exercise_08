cmake_minimum_required(VERSION 3.10)
project(oop_exercise_08)

set(CMAKE_CXX_STANDARD 17)
add_executable(oop_exercise_08 main.cpp subscriber.h)
find_package(Threads REQUIRED)
target_link_libraries(oop_exercise_08 ${CMAKE_THREAD_LIBS_INIT})

