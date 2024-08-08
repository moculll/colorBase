cmake_minimum_required(VERSION 3.12)


project(colorBase)


include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/include)
if(UNIX)

elseif(WIN32)
    link_libraries(winmm.lib)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

add_executable(colorBase ${CMAKE_SOURCE_DIR}/colorBase.cpp ${CMAKE_SOURCE_DIR}/example/main.cpp)
