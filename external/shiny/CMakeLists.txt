cmake_minimum_required( VERSION 2.8)

# include_directories (
#   "${PROJECT_SOURCE_DIR}/core/src"
#   "${PROJECT_SOURCE_DIR}/external"
#   )

set( shiny_src
  ShinyManager.cpp
  ShinyNode.cpp
  ShinyNodePool.cpp
  ShinyOutput.cpp
  ShinyTools.cpp
)
# add_subdirectory(../core)

add_library(shiny ${shiny_src})

