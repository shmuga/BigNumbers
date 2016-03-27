include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runBasicCalendarTests
        big_number.cpp)

target_link_libraries(runBasicCalendarTests gtest gtest_main)
target_link_libraries(runBasicCalendarTests src)
