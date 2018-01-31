cmake_minimum_required (VERSION 2.8)

project (FlightOS)
add_definitions("-std=c++11")

add_subdirectory(core)
add_subdirectory(external/shiny)

#TODO: create platform target settings
add_subdirectory(platform/win64) 