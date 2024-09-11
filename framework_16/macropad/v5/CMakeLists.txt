cmake_minimum_required(VERSION 3.13)
#set(PICO_SDK_PATH "path to pico sdk")
set(PICO_BOARD pico)

include(pico_sdk_import.cmake)
project(macropad1_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()


add_executable(macropad1
  macropad1.c
  serial.c
  i2c.c
  macropad_usb.c
  usb_descriptors.c
  msc_disk.c
  keyboard.c
  led.c
  sleep.c
  rosc.c
  )

pico_enable_stdio_usb(macropad1 1)

#pico_enable_stdio_uart(macropad1 1)
pico_add_extra_outputs(macropad1)
target_link_libraries(macropad1
  pico_stdlib
  hardware_clocks
  hardware_rtc
  hardware_adc
  hardware_gpio
  hardware_i2c
  pico_unique_id
  tinyusb_device
  tinyusb_board
  )

target_include_directories(macropad1 PRIVATE
                        .
                        $ENV{PICO_SDK_PATH}/lib/tinyusb/hw
                        $ENV{PICO_SDK_PATH}/lib/tinyusb/src
                        )

