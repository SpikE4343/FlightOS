cmake_minimum_required( VERSION 2.8)



include_directories (
  "${PROJECT_SOURCE_DIR}/core/src"
  "${PROJECT_SOURCE_DIR}/external"
  )

# set( sim_src )
add_subdirectory(../core)

add_library(core ${core_sources})
add_executable(flightos)

target_link_libraries(flightos core)

