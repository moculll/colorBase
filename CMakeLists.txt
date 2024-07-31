cmake_minimum_required(VERSION 3.12)


project(colorBase)


include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(colorBase ${CMAKE_SOURCE_DIR}/colorBase.cpp ${CMAKE_SOURCE_DIR}/example/main.cpp)
