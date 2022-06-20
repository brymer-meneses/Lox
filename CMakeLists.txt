cmake_minimum_required(VERSION 3.10)

project(Lox)

# NOTE: TW and BC are abbreviations for treewalker and bytecode respectively.

# Allow IDEs to get context from this build script
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(loxTW_MAIN "${PROJECT_SOURCE_DIR}/lox/src/main.cpp")
set(test_runner_MAIN "${PROJECT_SOURCE_DIR}/tests/test_runner.cpp")

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

# lox treewalker
file(GLOB loxTW_SOURCES "${PROJECT_SOURCE_DIR}/lox/src/*.cpp")
list(REMOVE_ITEM loxTW_SOURCES ${loxTW_MAIN}) # avoid 'main.c' from being added twice

add_library(loxTWlib STATIC ${loxTW_SOURCES})

# LINK EXECUTABLES
add_executable(loxTW ${loxTW_MAIN})
target_link_libraries(loxTW loxTWlib)

# COMPILE TESTS
file(GLOB
  TEST_FILES
  "${PROJECT_SOURCE_DIR}/lox/tests/*.cpp"
  "${PROJECT_SOURCE_DIR}/loxVM/tests/*.cpp"
)

add_executable(
  test_runner 
  ${test_runner_MAIN} 
  ${TEST_FILES}
)

target_link_libraries(
  test_runner 
  loxTWlib
  gtest_main 
  gmock_main
)
target_include_directories(
  test_runner 
  PUBLIC 
  ${PROJECT_SOURCE_DIR}/lox/src
  ${PROJECT_SOURCE_DIR}/loxVM/src
)

