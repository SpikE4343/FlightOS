cmake_minimum_required (VERSION 3.3)

set( FOS_LIBRARIES_SOURCE_DIR ${PROJECT_SOURCE_DIR}/libraries)

add_subdirectory(core)
add_subdirectory(web)
add_subdirectory(external)

#TODO: create platform target settings
add_subdirectory(platform/win64) 