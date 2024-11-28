set(PHYSICAL_ROBOT_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PHYSICAL_ROBOT_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find all source and header files
file(GLOB_RECURSE PHYSICAL_ROBOT_SOURCE_FILES ${PHYSICAL_ROBOT_SOURCE_DIRECTORY}/*.cc)
file(GLOB_RECURSE PHYSICAL_ROBOT_INCLUDE_FILES ${PHYSICAL_ROBOT_INCLUDE_DIRECTORY}/*.h)

add_library(${PHYSICAL_ROBOT_LIBRARY_NAME} STATIC
  ${PHYSICAL_ROBOT_SOURCE_FILES}
  ${PHYSICAL_ROBOT_INCLUDE_FILES}
)

# Should be public so when programs link against it include directories are propagated
target_include_directories(${PHYSICAL_ROBOT_LIBRARY_NAME} PUBLIC ${PHYSICAL_ROBOT_INCLUDE_DIRECTORY})

# Should be public so when programs link against it library functions are propagated
target_link_libraries(${PHYSICAL_ROBOT_LIBRARY_NAME} PUBLIC ${ROBOT_LIBRARY_NAME})