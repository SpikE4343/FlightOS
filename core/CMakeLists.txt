cmake_minimum_required( VERSION 2.8)

add_definitions("-std=c++11")

include_directories (
  "${PROJECT_SOURCE_DIR}/core/src"
  "${PROJECT_SOURCE_DIR}/external"
  )

message(${CMAKE_CURRENT_SOURCE_DIR})

set( core_sources 
  src/Application.cpp
  src/Asset.cpp
  src/AssetManager.cpp
  src/Config.cpp
  src/DeviceManager.cpp
  src/Hardware.cpp
  src/Input.cpp
  src/Log.cpp
  src/MemoryBuffer.cpp
  src/ModuleManager.cpp
  src/Platform.cpp
  src/Profiler.cpp
  src/System.cpp
  src/SystemModule.cpp
  src/Task.cpp
  src/TaskManager.cpp
  src/TaskQueue.cpp
  src/Types.cpp
  src/Util.cpp
)


add_library(core ${core_sources} ../external/shiny)

target_link_libraries(core shiny)

