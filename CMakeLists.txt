# Project Initialization

cmake_minimum_required(VERSION 3.10)
project(AgentOOP
  VERSION 0.1.0
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Library for actor class and behavior

set(LIBNAME "agentlib")

add_library(${LIBNAME} SHARED)

target_include_directories(${LIBNAME} PUBLIC include)

target_sources(${LIBNAME}
  PRIVATE
  #source targets
  
  # agents
  src/agents/Agent.cpp
  src/agents/AgentGraph.cpp
  
  # graphs
  src/graphs/SpatialGraph.cpp
  
  # core
  src/core/EntityManager.cpp
  src/core/ComponentManager.cpp
  
  # include targets

  
  src/agents/Agent.hpp
  src/agents/AgentGraph.hpp
  src/graphs/SpatialGraph.hpp
)


# Main application

add_executable(AgentOOP
  src/main.cpp
)

target_link_libraries(AgentOOP PRIVATE ${LIBNAME})


