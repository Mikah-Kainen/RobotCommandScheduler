cmake_minimum_required(VERSION 3.16)
project(RobotCommandScheduler)

# Set subproject directories
set(SCHEDULER_LIBRARY_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scheduler)
set(SCHEDULER_TESTS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(ROBOT_LIBRARY_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/robot_library)
set(VIRTUAL_ROBOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/virtual_robot)
set(PHYSICAL_ROBOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/physical_robot)
set(MAIN_EXECUTABLE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/main)

# Set output names
set(SCHEDULER_LIBRARY_NAME scheduler_binary)
set(SCHEDULER_TESTS_NAME scheduler_tests_build)
set(ROBOT_LIBRARY_NAME robot_library_binary)
set(PHYSICAL_ROBOT_LIBRARY_NAME physical_robot_library_binary)
set(VIRTUAL_ROBOT_LIBRARY_NAME virtual_robot_library_binary)
set(MAIN_EXECUTABLE_NAME main_build)

option(BUILD_PHYSICAL "Build for physical robot" OFF)

if (BUILD_PHYSICAL)
  message(STATUS "BUILD_PHYSICAL is ON: Building through Docker.")
  set(CMAKE_SKIP_BUILD_RPATH YES)
else()
  message(STATUS "BUILD_PHYSICAL is OFF: Setting up virtual tests.")
endif()

if (BUILD_PHYSICAL)
  # set(CMAKE_C_COMPILER "arm-linux-gnueabi-gcc")
  # set(CMAKE_CXX_COMPILER "arm-linux-gnueabi-g++")
  set(CMAKE_TOOLCHAIN_FILE "/path/to/your/toolchain.cmake")
else()
  # Compiler settings
  # GoogleTest requires at least C++14
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -MD -MP")
endif()

# if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
#   # Set compiler flags for x86_64 architecture
#   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=x86-64")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64")
# elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
#   # Set compiler flags for ARM 64-bit architecture
#   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a")
# else()
#   message(STATUS "UNKOWN SYSTEM PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}.")
# endif()

if (NOT BUILD_PHYSICAL)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )

  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()
endif()

add_subdirectory(${SCHEDULER_LIBRARY_DIRECTORY})
add_subdirectory(${ROBOT_LIBRARY_DIRECTORY})

if (BUILD_PHYSICAL)
  add_subdirectory(${PHYSICAL_ROBOT_DIRECTORY})
else()
  add_subdirectory(${VIRTUAL_ROBOT_DIRECTORY})
endif()

add_subdirectory(${MAIN_EXECUTABLE_DIRECTORY})

if (NOT BUILD_PHYSICAL)
  add_subdirectory(${SCHEDULER_TESTS_DIRECTORY})
endif()

# if(RUN_GMR_MAIN)
# else()
#   add_subdirectory(${SCHEDULER_TESTS_DIRECTORY})
# endif()





# # Set default build type
# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Debug)
# endif()

# # Define options for build targets
# option(BUILD_FOR_PHYSICAL "Build for the physical robot" OFF) # Default OFF

# add_subdirectory(${SCHEDULER_LIBRARY_DIRECTORY})
# add_subdirectory(${SHARED_FILES_DIRECTORY})

# if(BUILD_FOR_PHYSICAL)
# message(STATUS "Building for the physical robot.")
# add_subdirectory(${PHYSICAL_ROBOT_DIRECTORY})
# else()
# message(STATUS "Building for the virtual robot.")

# # Only include Google Test when running the virtual robot
# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )

# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# enable_testing()

# add_subdirectory(${VIRTUAL_ROBOT_DIRECTORY})
# add_subdirectory(${SCHEDULER_TESTS_DIRECTORY})
# endif()

# add_subdirectory(${MAIN_EXECUTABLE_DIRECTORY})