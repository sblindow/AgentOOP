# Project Initialization

cmake_minimum_required(VERSION 3.10)
project(AgentOOP
  VERSION 0.1.0
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding Raylib

set(RAYLIB_VERSION 6.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build examples
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # don't build example games
    set(CUSTOMIZE_BUILD ON CACHE BOOL "" FORCE)
    set(OPENGL_VERSION "3.3" CACHE STRING "" FORCE)
    FetchContent_MakeAvailable(raylib)
    
  endif()
endif()


# Library for actor class and behavior
set(LIBNAME "agentlib")
add_library(${LIBNAME} SHARED)
target_include_directories(${LIBNAME} PUBLIC
   include
   src
)
target_sources(${LIBNAME}
  PRIVATE
  #source targets
  
  # agents
  src/agents/Agent.cpp
  src/agents/AgentGraph.cpp
  
  # graphs
  src/graphs/SpatialGraph.cpp

  #systems


  # game
  src/game/Player.cpp

  # include targets
  src/agents/Agent.hpp
  src/agents/AgentGraph.hpp
  src/graphs/SpatialGraph.hpp
)
target_link_libraries(${LIBNAME} PUBLIC raylib)

# Main application
add_executable(AgentOOP
  src/main.cpp
)
target_link_libraries(AgentOOP PRIVATE ${LIBNAME})
