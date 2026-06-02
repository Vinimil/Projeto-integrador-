cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(proj C CXX ASM)

set(CMAKE_C_STANDARD 11)

pico_sdk_init()

add_executable(proj
    main.c
    adc.c
)

target_include_directories(proj PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(proj
    pico_stdlib
    hardware_adc
)

pico_enable_stdio_usb(proj 1)
pico_enable_stdio_uart(proj 0)

pico_add_extra_outputs(proj)
