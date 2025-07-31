# Project Initialization

cmake_minimum_required(VERSION 3.10)
project(ActorOOP
  VERSION 0.1.0
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)


# Library for actor class and behavior

set(LIBNAME "actorlib")
add_library(${LIBNAME} SHARED)
target_include_directories(${LIBNAME} PUBLIC include)
target_sources(${LIBNAME}
  PRIVATE
  src/Agent.cpp
  include/Agent.h
)


# Main application
add_executable(ActorOOP
  src/main.cpp
)
target_link_libraries(ActorOOP PRIVATE ${LIBNAME})


