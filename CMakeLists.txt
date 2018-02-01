cmake_minimum_required (VERSION 3.9)

project (FlightOS)

set(CMAKE_COLOR_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 14)


# include(Macros)
# today(DATE)
# now(TIME)
# git_id(GIT_STR)



add_subdirectory(libraries)
add_subdirectory(simulator)
add_subdirectory(configurator)