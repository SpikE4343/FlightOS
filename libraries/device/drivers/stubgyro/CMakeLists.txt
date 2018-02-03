cmake_minimum_required( VERSION 3.3)

include_directories (
  "${FOS_LIBRARIES_SOURCE_DIR}/core/src"
  "${FOS_LIBRARIES_SOURCE_DIR}/external"
  "${FOS_LIBRARIES_SOURCE_DIR}/device/src"
)

set( stubgyro_headers 
)

set( stubgyro_sources 
  src/Gyro.cpp
)


add_library(stubgyro ${stubgyro_sources} ${FOS_LIBRARIES_SOURCE_DIR}/core)

target_link_libraries(stubgyro core)