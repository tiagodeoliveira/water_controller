cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME "water_controller")

set(IDF_SDKCONFIG_DEFAULTS ${CMAKE_CURRENT_LIST_DIR}/sdkconfig.defaults)

project(${PROJECT_NAME})

file(GLOB SOURCES "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

set(INCLUDE_DIRS ${INCLUDE_DIRS} "include/")
include_directories(${INCLUDE_DIRS})

set(IDF_PROJECT_EXECUTABLE ${PROJECT_NAME})

set(AFR_BOARD espressif.esp32_devkitc CACHE INTERNAL "")
add_subdirectory(freertos)

target_link_libraries(${PROJECT_NAME} PRIVATE
        AFR::wifi
        AFR::utils
        AFR::common
        AFR::pkcs11
        AFR::platform
        AFR::mqtt
        AFR::ota
        3rdparty::jsmn
)

