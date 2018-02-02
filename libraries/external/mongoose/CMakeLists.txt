cmake_minimum_required( VERSION 2.8)

# include_directories (
#   "${PROJECT_SOURCE_DIR}/core/src"
#   "${PROJECT_SOURCE_DIR}/external"
#   )

set( mongoose_src
  mongoose.c
)
# add_subdirectory(../core)

add_library(mongoose ${mongoose_src})

