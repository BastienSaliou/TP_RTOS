cmake_minimum_required(VERSION 3.26)
project(Free_RTOS C)

set(CMAKE_C_STANDARD 11)

# Exécutable pour tester Byte
add_executable(test_byte
        Byte.c
        Test_byte.c
        Test_lib.c

)

# Exécutable pour tester Package
add_executable(test_package
        Byte.c
        package.c
        Test_package.c
        Test_lib.c
)
add_executable(test_mux_demux
        Byte.c
        package.c
        mux_demux.c
        Test_lib.c
        Test_mux_demux.c)
