# Directories
set(TEST_SRC_DIR src)
set(TEST_INC_DIR inc)
set(TEST_EXEC rcs_tests)

# Find all source files
file(GLOB_RECURSE TEST_SOURCE_FILES ${TEST_SRC_DIR}/*.cc)

add_executable(
  ${TEST_EXEC}
  ${TEST_SOURCE_FILES}
)
target_link_libraries(
  ${TEST_EXEC}
  GTest::gtest_main
)

target_include_directories(${TEST_EXEC} PRIVATE ${TEST_INC_DIR})

include(GoogleTest)
gtest_discover_tests(${TEST_EXEC})