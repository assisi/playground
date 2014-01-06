cmake_minimum_required(VERSION 2.8)

project(assisi_playground)

# Add custom modules for finding dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      "${CMAKE_SOURCE_DIR}/cmake/Modules")

add_subdirectory(playground)
