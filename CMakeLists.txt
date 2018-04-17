cmake_minimum_required(VERSION 3.10)
project(dlib_1)

set(CMAKE_CXX_STANDARD 14)

find_package(dlib REQUIRED)

add_executable(dlib_1 main.cpp points.h)
target_link_libraries(${PROJECT_NAME} dlib::dlib)