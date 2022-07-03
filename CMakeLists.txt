cmake_minimum_required(VERSION 3.10)

project(Lox)

add_compile_options(-g -Wall -Wextra)

# NOTE: TW and BC are abbreviations for treewalker and bytecode respectively.

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(clox_MAIN "${PROJECT_SOURCE_DIR}/src/main.c")
set(test_runner_MAIN "${PROJECT_SOURCE_DIR}/tests/runner.cpp")

# HANDLE DEPENDENCIES
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG        release-1.11.0
)
FetchContent_GetProperties(googletest)

if (NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()

# COMPILE LIBRARIES

# clox
file(GLOB clox_SOURCES "${PROJECT_SOURCE_DIR}/src/lox/*.c" "${PROJECT_SOURCE_DIR}/src/tools/*.c")
add_library(cloxlib STATIC ${clox_SOURCES})
target_include_directories(cloxlib PUBLIC ${PROJECT_SOURCE_DIR}/include/)

# LINK EXECUTABLES
add_executable(clox ${clox_MAIN})
target_link_libraries(clox cloxlib)
target_include_directories(clox PUBLIC ${PROJECT_SOURCE_DIR}/include/)

# COMPILE TESTS
file(GLOB
  TEST_FILES
  "${PROJECT_SOURCE_DIR}/tests/*.cpp"
)

add_executable(
  test_runner 
  ${test_runner_MAIN} 
  ${TEST_FILES}
)

target_link_libraries(
  test_runner 
  cloxlib 
  gtest_main 
  gmock_main
)
target_include_directories(
  test_runner 
  PUBLIC 
  ${PROJECT_SOURCE_DIR}/include/
)

