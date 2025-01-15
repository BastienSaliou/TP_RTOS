cmake_minimum_required(VERSION 3.26)
project(Free_RTOS C)

set(CMAKE_C_STANDARD 11)

add_executable(test_byte
        Test_byte.c
        Byte.c
        Test_lib.c
)
target_compile_definitions(test_byte PRIVATE TEST_BYTE_MAIN)

add_executable(test_mux_demux
        Test_mux_demux.c
        mux_demux.c
        package.c
        Test_lib.c
        Byte.c
)
target_compile_definitions(test_mux_demux PRIVATE TEST_MUX_DEMUX_MAIN)

add_executable(test_package
        Test_package.c
        package.c
        Byte.c
        Test_lib.c
)
target_compile_definitions(test_package PRIVATE TEST_PACKAGE_MAIN)

add_executable(queue_management
        queue_management.c
        Byte.c
        mux_demux.c
        package.c
        Test_lib.c
)


add_executable(test_global
        Test_global.c
        Byte.c
        mux_demux.c
        package.c
        Test_byte.c
        Test_mux_demux.c
        Test_package.c
        Test_lib.c
)
