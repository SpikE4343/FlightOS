cmake_minimum_required( VERSION 3.3)


include_directories (
  "${FOS_LIBRARIES_SOURCE_DIR}/core/src"
  "${FOS_LIBRARIES_SOURCE_DIR}/external"
  )

set( flight_headers 
)

set( flight_sources 
  src/FlightController.cpp
)


add_library(flight ${flight_sources} ${FOS_LIBRARIES_SOURCE_DIR}/core)

target_link_libraries(flight core)

