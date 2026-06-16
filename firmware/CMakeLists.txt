# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)

set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)

if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(proj C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(proj

    main.c

    adc.c
    calibracao.c

    lcd_i2c.c
)

target_include_directories(proj PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(proj

    pico_stdlib

    hardware_adc
    hardware_gpio
    hardware_i2c
)

pico_enable_stdio_usb(proj 1)
pico_enable_stdio_uart(proj 0)

pico_add_extra_outputs(proj)
