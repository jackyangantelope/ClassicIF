cmake_minimum_required(VERSION 3.20)
set(PICO_BOARD adafruit_fruit_jam CACHE STRING "Adafruit Fruit Jam" FORCE)
set(PICO_PSRAM_CS_PIN 47 CACHE STRING "Fruit Jam PSRAM chip select" FORCE)
set(PICO_PSRAM_SIZE_BYTES 0x800000 CACHE STRING "Fruit Jam PSRAM size" FORCE)
set(PICO_DEFAULT_UART 0 CACHE STRING "Diagnostic UART" FORCE)
set(PICO_DEFAULT_UART_TX_PIN 44 CACHE STRING "Diagnostic UART TX" FORCE)
set(PICO_DEFAULT_UART_RX_PIN 45 CACHE STRING "Diagnostic UART RX" FORCE)
set(PICO_FLASH_SPI_CLKDIV 10 CACHE STRING "Conservative XIP boot divider" FORCE)
if(NOT DEFINED PICO_SDK_PATH)
    message(FATAL_ERROR "Set PICO_SDK_PATH to Pico SDK 2.3.0 or newer")
endif()
set(PICO_SHARED_PATH "${CMAKE_CURRENT_LIST_DIR}/deps/pico_shared" CACHE PATH "pico_shared source")
if(NOT DEFINED PICO_PIO_USB_PATH)
    message(FATAL_ERROR "Set PICO_PIO_USB_PATH to a Pico-PIO-USB checkout")
endif()
include("${PICO_SDK_PATH}/external/pico_sdk_import.cmake")
project(picoClassicIF C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

set(GLULXE_ROOT "${CMAKE_CURRENT_LIST_DIR}/deps/glulxe")
set(CHEAPGLK_ROOT "${CMAKE_CURRENT_LIST_DIR}/deps/cheapglk")

set(CHEAPGLK_SOURCES
    cgfref.c cggestal.c cgmisc.c cgstream.c cgstyle.c cgwindow.c cgschan.c
    cgunicod.c main.c gi_dispa.c gi_blorb.c gi_debug.c cgblorb.c)
list(TRANSFORM CHEAPGLK_SOURCES PREPEND "${CHEAPGLK_ROOT}/")
add_library(cheapglk_embedded STATIC ${CHEAPGLK_SOURCES}
    "${CMAKE_CURRENT_LIST_DIR}/firmware/cgdate_embedded.c")
set_source_files_properties("${CHEAPGLK_ROOT}/main.c" PROPERTIES COMPILE_DEFINITIONS "main=cheapglk_main")
target_include_directories(cheapglk_embedded PUBLIC "${CHEAPGLK_ROOT}" "${CMAKE_CURRENT_LIST_DIR}/platform/wave1")
target_compile_definitions(cheapglk_embedded PRIVATE
    malloc=jack_psram_malloc calloc=jack_psram_calloc realloc=jack_psram_realloc free=jack_psram_free)
target_compile_options(cheapglk_embedded PRIVATE -Wno-unused-parameter -Wno-sign-compare)

set(GLULXE_SOURCES main.c files.c vm.c exec.c funcs.c operand.c string.c glkop.c
    heap.c serial.c search.c accel.c float.c gestalt.c osdepend.c profile.c
    debugger.c unixstrt.c)
list(TRANSFORM GLULXE_SOURCES PREPEND "${GLULXE_ROOT}/")
add_library(glulxe_core STATIC ${GLULXE_SOURCES})
target_include_directories(glulxe_core PUBLIC "${GLULXE_ROOT}" "${CHEAPGLK_ROOT}" "${CMAKE_CURRENT_LIST_DIR}/platform/wave1")
target_compile_definitions(glulxe_core PRIVATE OS_UNIX=1
    malloc=jack_psram_malloc calloc=jack_psram_calloc realloc=jack_psram_realloc free=jack_psram_free)
target_compile_options(glulxe_core PRIVATE -Wno-unused-parameter -Wno-sign-compare -Wno-unused-function)

add_library(classicif_text STATIC
    "${CMAKE_CURRENT_LIST_DIR}/platform/text/text_console.c")
target_include_directories(classicif_text PUBLIC "${CMAKE_CURRENT_LIST_DIR}/platform/text")
set(JACK_WAVE1_HDMI_AUDIO OFF)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/platform/wave1" "${CMAKE_BINARY_DIR}/wave1")
target_compile_definitions(jack_wave1_runtime PRIVATE
    JACK_WAVE1_SYS_CLOCK_KHZ=252000u
    JACK_WAVE1_FLASH_MAX_HZ=42000000u
    JACK_WAVE1_PSRAM_MAX_HZ=96000000u)
target_link_libraries(jack_wave1_runtime PRIVATE hardware_pll hardware_psram)
option(BUILD_FOR_BOOTLOADER "Link to the resident loader app partition" ON)
add_executable(picoClassicIF
    "${CMAKE_CURRENT_LIST_DIR}/firmware/playable_platform.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/platform/wave1/jack_psram_heap.cpp")
pico_set_program_name(picoClassicIF "picoClassicIF")
pico_set_program_version(picoClassicIF "0.1.0-fruit-jam-source-candidate")
pico_enable_stdio_uart(picoClassicIF 1)
pico_enable_stdio_usb(picoClassicIF 0)
target_compile_definitions(picoClassicIF PRIVATE HW_CONFIG=8 PICO_STACK_SIZE=4096
    PICO_FLASH_SPI_CLKDIV=10 PICO_PSRAM_CS_PIN=47 PICO_PSRAM_SIZE_BYTES=0x800000)
target_link_libraries(picoClassicIF PRIVATE pico_stdlib hardware_psram glulxe_core cheapglk_embedded glulxe_core cheapglk_embedded classicif_text jack_wave1_runtime)
target_link_options(picoClassicIF PRIVATE -Wl,--print-memory-usage)
if(BUILD_FOR_BOOTLOADER)
    include("${CMAKE_CURRENT_LIST_DIR}/cmake/LoaderPartition.cmake")
    classicif_link_for_loader(picoClassicIF)
endif()
pico_add_extra_outputs(picoClassicIF)
