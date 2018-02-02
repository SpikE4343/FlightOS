cmake_minimum_required( VERSION 3.3)


include_directories (
  "${FOS_LIBRARIES_SOURCE_DIR}/core/src"
  "${FOS_LIBRARIES_SOURCE_DIR}/external"
  )

set( win64_headers 
)

set( win64_sources 
  src/Win64Platform.cpp
)


add_library(win64_platform ${win64_sources} ${FOS_LIBRARIES_SOURCE_DIR}/core)

target_link_libraries(win64_platform core)

