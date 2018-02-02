cmake_minimum_required (VERSION 3.3)

project (FlightOS)

set(CMAKE_COLOR_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 14)
#set_property( GLOBAL PROPERTY USE_FOLDERS ON)

# include(Macros)
# today(DATE)
# now(TIME)
# git_id(GIT_STR)



add_subdirectory(libraries)
add_subdirectory(targets)