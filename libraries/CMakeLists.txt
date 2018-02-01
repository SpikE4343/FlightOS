cmake_minimum_required (VERSION 3.9)

set( FOS_LIBRARIES_SOURCE_DIR ${PROJECT_SOURCE_DIR}/libraries)

add_subdirectory(external/shiny)
add_subdirectory(core)

#TODO: create platform target settings
add_subdirectory(platform/win64) 