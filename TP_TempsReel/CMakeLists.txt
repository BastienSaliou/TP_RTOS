cmake_minimum_required(VERSION 3.26)
project(Free_RTOS C)

set(CMAKE_C_STANDARD 11)

add_executable(Free_RTOS main.c
        Byte.c
        Test_lib.h
        Test_lib.c
        Byte.h
        package.h
        package.c
        mux_demux.c
        mux_demux.h
        Test_byte.c
        Test_byte.h)
