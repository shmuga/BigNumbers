include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(BigNumberTests
        BigNumber.spec.cpp)

target_link_libraries(BigNumberTests gtest gtest_main)
target_link_libraries(BigNumberTests src)
