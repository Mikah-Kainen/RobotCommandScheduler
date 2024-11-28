cmake_minimum_required(VERSION 3.16)

# Set subproject directories
set(SCHEDULER_LIBRARY_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scheduler)
set(SCHEDULER_TESTS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(VIRTUAL_ROBOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/virtual_robot)
set(PHYSICAL_ROBOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/physical_robot)
set(SHARED_FILES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/common)

# Set output names
set(PROJECT_EXECUTABLE_NAME rcs)
set(SCHEDULER_LIBRARY_NAME scheduler_build)
set(SCHEDULER_TESTS_NAME scheduler_tests_build)
set(PHYSICAL_ROBOT_EXECUTABLE_NAME physical_robot_build)
set(VIRTUAL_ROBOT_EXECUTABLE_NAME virtual_robot_build)

project(${PROJECT_EXECUTABLE_NAME})

# Compiler settings
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -MD -MP")

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Define options for build targets
option(BUILD_FOR_PHYSICAL "Build for the physical robot" OFF) # Default OFF

add_subdirectory(${SCHEDULER_LIBRARY_DIRECTORY})
add_subdirectory(${SHARED_FILES_DIRECTORY})

if(BUILD_FOR_PHYSICAL)
message(STATUS "Building for the physical robot.")
add_subdirectory(${PHYSICAL_ROBOT_DIRECTORY})
else()
message(STATUS "Building for the virtual robot.")

# Only include Google Test when running the virtual robot
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(${VIRTUAL_ROBOT_DIRECTORY})
add_subdirectory(${SCHEDULER_TESTS_DIRECTORY})
endif()