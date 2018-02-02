cmake_minimum_required( VERSION 3.3)


include_directories (
  "${FOS_LIBRARIES_SOURCE_DIR}/core/src"
  "${FOS_LIBRARIES_SOURCE_DIR}/external"
  )

set( web_headers 
)

set( web_sources 
  src/Web.cpp
)


add_library(web 
  ${web_sources} 
  ${FOS_LIBRARIES_SOURCE_DIR}/core
  ${FOS_LIBRARIES_SOURCE_DIR}/external/mongoose )

target_link_libraries(web core mongoose)

