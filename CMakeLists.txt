cmake_minimum_required(VERSION 3.10)
project(kkmeans)

set(CMAKE_CXX_STANDARD 14)

find_package(dlib REQUIRED)

add_executable(kkmeans main.cpp)
target_link_libraries(${PROJECT_NAME} dlib::dlib)