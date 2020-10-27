cmake_minimum_required(VERSION 3.13)

project(water_controller)

add_executable(controller src/main.c)

set(IDF_PROJECT_EXECUTABLE controller)

get_filename_component(
    EXTRA_COMPONENT_DIRS
    "components/foo" ABSOLUTE
)
list(APPEND IDF_EXTRA_COMPONENT_DIRS ${EXTRA_COMPONENT_DIRS})

include_directories(BEFORE freertos-configs)

set(AFR_BOARD espressif.esp32_devkitc CACHE INTERNAL "")
add_subdirectory(freertos)

target_link_libraries(controller PRIVATE AFR::demo_mqtt)
target_link_libraries(controller PRIVATE AFR::demo_ble_numeric_comparison)