cmake_minimum_required(VERSION 3.10)

project(Lox)

# NOTE: TW and BC are abbreviations for treewalker and bytecode respectively.

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(cloxTW_MAIN "${PROJECT_SOURCE_DIR}/treewalker/src/main.c")
set(test_runner_MAIN "${PROJECT_SOURCE_DIR}/utils/src/runner.cpp")

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

# clox treewalker
file(GLOB cloxTW_SOURCES "${PROJECT_SOURCE_DIR}/treewalker/src/*.c")
list(REMOVE_ITEM cloxTW_SOURCES ${cloxTW_MAIN}) # avoid 'main.c' from being added twice
add_library(cloxTWlib STATIC ${cloxTW_SOURCES})
target_include_directories(cloxTWlib PUBLIC ${PROJECT_SOURCE_DIR}/utils/src)

# utils 
file(GLOB util_SOURCES "${PROJECT_SOURCE_DIR}/utils/src/*.c")
add_library(utilslib STATIC ${util_SOURCES})

# LINK EXECUTABLES
add_executable(cloxTW ${cloxTW_MAIN})
target_link_libraries(cloxTW cloxTWlib utilslib)

# COMPILE TESTS
file(GLOB
  TEST_FILES
  "${PROJECT_SOURCE_DIR}/treewalker/tests/*.cpp"
  "${PROJECT_SOURCE_DIR}/bytecode/tests/*.cpp"
  "${PROJECT_SOURCE_DIR}/utils/tests/*.cpp"
)

add_executable(
  test_runner 
  ${test_runner_MAIN} 
  ${TEST_FILES}
)

target_link_libraries(
  test_runner 
  cloxTWlib 
  utilslib
  gtest_main 
  gmock_main
)
target_include_directories(
  test_runner 
  PUBLIC 
  ${PROJECT_SOURCE_DIR}/treewalker/src
  ${PROJECT_SOURCE_DIR}/bytecode/src
  ${PROJECT_SOURCE_DIR}/utils/src
)

