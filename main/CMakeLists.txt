set(INCLUDEDIRS
    ./
)

set(SRCS
    main.cpp
    test_nvs_wifi.cpp
    test_bluetooth_serial.cpp
    test_uart_unitgps.cpp
    test_i2c_imu6866.cpp
    test_wasm3_clockenv.cpp
    test_wasm3_imu6886.cpp
    test_wasm3_gpsgsv.cpp
    test_freetype.cpp
)

idf_component_register(
    INCLUDE_DIRS ${INCLUDEDIRS}
    SRCS ${SRCS}
    REQUIRES arduino m5stack wasm lwgps)
