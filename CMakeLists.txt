cmake_minimum_required(VERSION 2.8)
project(Calendar)

add_definitions(-std=c++11)

set(SOURCE_FILES main.cpp)
add_executable(run ${SOURCE_FILES})

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(run src)
