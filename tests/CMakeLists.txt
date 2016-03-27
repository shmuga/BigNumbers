
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/gtest-1.7.0 ${CMAKE_CURRENT_BINARY_DIR}/gtRest)
include_directories(${CMAKE_SOURCE_DIR}/lib/gtest-1.7.0/include ${CMAKE_SOURCE_DIR}/lib/gtest)

set(SOURCE_FILES
        BigNumber.spec.cpp)
enable_testing()
add_executable(Test ${SOURCE_FILES})
#target_link_libraries(Test ${GTEST_BOTH_LIBRARIES})