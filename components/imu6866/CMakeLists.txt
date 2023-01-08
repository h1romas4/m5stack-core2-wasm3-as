cmake_minimum_required(VERSION 3.5)

set(INCLUDEDIRS
    MadgwickAHRS/src
)

set(SRCS
    MadgwickAHRS/src/MadgwickAHRS.cpp
)

idf_component_register(INCLUDE_DIRS ${INCLUDEDIRS} SRCS ${SRCS})

target_compile_options(${COMPONENT_TARGET} PUBLIC
    -O3
)
