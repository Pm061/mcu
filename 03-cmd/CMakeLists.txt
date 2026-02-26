cmake_minimum_required(VERSION 3.15)

set(PICO_BOARD pico)
set(PICO_PLATFORM rp2040)

include(pico_sdk_import.cmake)

project(03_cmd C CXX)
pico_sdk_init()

add_executable(03_cmd
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

target_link_libraries(03_cmd
    pico_stdlib
    hardware_gpio
)

pico_enable_stdio_usb(03_cmd 1)
pico_enable_stdio_uart(03_cmd 0)

pico_add_extra_outputs(03_cmd)