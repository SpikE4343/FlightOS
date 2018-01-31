cmake_minimum_required( VERSION 2.8)

#add_definitions("-std=c++11")

include_directories (
  "${PROJECT_SOURCE_DIR}/core/src"
  "${PROJECT_SOURCE_DIR}/external"
  )

#message(${CMAKE_CURRENT_SOURCE_DIR})

set( win64_headers 
)

set( win64_sources 
  src/Win64Platform.cpp
)


add_library(win64_platform ${win64_sources} ../core)

target_link_libraries(win64_platform core)

