cmake_minimum_required(VERSION 2.8)

project(src)

add_definitions(-std=c++11)

set(HEADER_FILES
        BigNumber/BigNumber.h
    )

set(SOURCE_FILES
        BigNumber/BigNumber.cpp
    )

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
