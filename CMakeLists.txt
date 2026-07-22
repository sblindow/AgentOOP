# Project Initialization

cmake_minimum_required(VERSION 3.10)
project(AgentOOP
  VERSION 0.1.0
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # don't build the supplied example games
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE) # enable wayland on Linux
set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE) # enable x11 on linux

FetchContent_Declare(
  raylib
  GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
  GIT_TAG "master"
  GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)


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
