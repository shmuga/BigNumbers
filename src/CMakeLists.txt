cmake_minimum_required(VERSION 2.8)

project(src)

add_definitions(-std=c++11)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../tests/cmake/")
find_package(GMP REQUIRED)
INCLUDE_DIRECTORIES(${GMP_INCLUDES})


set(HEADER_FILES
        BigNumber/BigNumber.h
        BigNumber/Functions.h
        BigNumber/BBS.h
        BigNumber/Gost.h
        BigNumber/elg.h)

set(SOURCE_FILES
        BigNumber/BigNumber.cpp
        BigNumber/Functions.cpp
        BigNumber/BBS.cpp
        BigNumber/Gost.cpp
        BigNumber/elg.cpp)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(src ${GMP_LIBRARIES})