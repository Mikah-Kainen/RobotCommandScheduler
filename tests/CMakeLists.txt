# Directories
set(TESTS_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TESTS_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find all source and header files
file(GLOB_RECURSE TESTS_SOURCE_FILES ${TESTS_SOURCE_DIRECTORY}/*.cc)
file(GLOB_RECURSE TESTS_INCLUDE_FILES ${TESTS_INCLUDE_DIRECTORY}/*.h)

message(STATUS "Scheduler tests target name: ${SCHEDULER_TESTS_NAME}")
message(STATUS "Source files: ${TESTS_SOURCE_FILES}")
execute_process(COMMAND sleep 5)

add_executable(
  ${SCHEDULER_TESTS_NAME}
  ${TESTS_SOURCE_FILES}
  ${TESTS_INCLUDE_FILES}
)

# Nothing should need the test include directories presently but in case something does in the future I am leaving it public
target_include_directories(${SCHEDULER_TESTS_NAME} PUBLIC ${TESTS_INCLUDE_DIRECTORY})

# Nothing should link against libraries presently but in case something does in the future I am leaving it public
target_link_libraries(
  ${SCHEDULER_TESTS_NAME} PUBLIC
  GTest::gtest_main
  ${SCHEDULER_LIBRARY_NAME}
  #${VIRTUAL_ROBOT_EXECUTABLE_NAME}
  virtual_robot_lib # Temporary lib while deciding how to organize robot code
)

include(GoogleTest)
gtest_discover_tests(${SCHEDULER_TESTS_NAME})