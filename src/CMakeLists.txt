set(SOURCE_FILES
        BigNumber.cpp
        BigNumber.h)

add_library(BigNumber ${SOURCE_FILES})
add_executable(BigNumbers main.cpp)