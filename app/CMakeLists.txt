cmake_minimum_required(VERSION 3.20.0)

# Tell Zephyr where our custom out-of-tree module is located
# (Assumes led_sensor_module is up one directory from the app folder)
list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/../led_sensor_module)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(led_sensor_app)

target_sources(app PRIVATE src/main.cpp)