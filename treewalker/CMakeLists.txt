cmake_minimum_required(VERSION 3.10)

project(clox)

file(GLOB sources 
  "${PROJECT_SOURCE_DIR}/src/*.c"
  "${PROJECT_SOURCE_DIR}/src/*.h"
)
add_executable(clox ${sources})

target_sources(clox PUBLIC ${sources})

