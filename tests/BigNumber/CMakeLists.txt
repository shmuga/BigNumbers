include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(BigNumberTests
        BigNumber.spec.cpp Functions.spec.cpp)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
find_package(GMP REQUIRED)

INCLUDE_DIRECTORIES(${GMP_INCLUDES})

target_link_libraries(BigNumberTests ${GMP_LIBRARIES})
target_link_libraries(BigNumberTests gtest gtest_main)
target_link_libraries(BigNumberTests src)
