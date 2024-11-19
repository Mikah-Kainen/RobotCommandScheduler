cmake_minimum_required(VERSION 3.10)

# Project setup
project(rcs LANGUAGES CXX)

# Directories
set(SRC_DIR src)
set(OBJ_DIR obj)
set(INC_DIR inc)
set(LIB_DIR lib)
set(EXEC rcs)

# Compiler settings
set(CMAKE_CXX_STANDARD 17) # Adjust as needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -MD -MP")

# Include directories
file(GLOB_RECURSE INCLUDE_DIRS ${INC_DIR}/*)
include_directories(${INCLUDE_DIRS})

# Find all .cc source files in SRC_DIR
file(GLOB_RECURSE ccs ${SRC_DIR}/*.cc)

# Create target executable
add_executable(${EXEC} ${ccs})

# Debug targets for printing lists
add_custom_target(debug_ccs
    COMMAND ${CMAKE_COMMAND} -E echo "ccs: ${ccs}"
)

# Clean targets
add_custom_target(cleanAll
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/${EXEC}
)

# cmake_minimum_required(VERSION 3.14)
# project(rcs)

# # GoogleTest requires at least C++14
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# enable_testing()

# add_executable(
#   hello_test
#   hello_test.cc
# )
# target_link_libraries(
#   hello_test
#   GTest::gtest_main
# )

# include(GoogleTest)
# gtest_discover_tests(hello_test)