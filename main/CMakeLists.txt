set(INCLUDEDIRS
    ./
)

set(SRCS
    main.cpp
    test_wasm3_clockenv.cpp
    test_freetype.cpp
)

idf_component_register(
    INCLUDE_DIRS ${INCLUDEDIRS}
    SRCS ${SRCS}
    REQUIRES arduino m5stack wasm)
