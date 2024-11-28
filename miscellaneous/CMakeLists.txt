cmake_minimum_required(VERSION 3.14)

# Project setup
project(rcs LANGUAGES CXX)

# Directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_SOURCE_DIR}/inc)
set(EXEC rcs)

# Compiler settings
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -MD -MP")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Find all source files
file(GLOB_RECURSE SOURCE_FILES ${SRC_DIR}/*.cc)

add_subdirectory(tests)

add_executable(
  ${EXEC}
  ${SOURCE_FILES}
)

# Automatically include the directory of each header
# foreach(header ${HEADER_FILES})
#     get_filename_component(header_dir ${header} DIRECTORY)
#     list(APPEND INCLUDE_DIRS ${header_dir})
# endforeach()
# list(REMOVE_DUPLICATES INCLUDE_DIRS)
# target_include_directories(${EXEC} PRIVATE ${INCLUDE_DIRS})
target_include_directories(${EXEC} PRIVATE ${INC_DIR} ${SRC_DIR})

# Define build options (Debug/Release)
#set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type: Debug or Release")
#set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")