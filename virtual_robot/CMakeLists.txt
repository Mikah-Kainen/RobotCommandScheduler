set(VIRTUAL_ROBOT_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(VIRTUAL_ROBOT_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find all source and header files
file(GLOB_RECURSE VIRTUAL_ROBOT_SOURCE_FILES ${VIRTUAL_ROBOT_SOURCE_DIRECTORY}/*.cc)
file(GLOB_RECURSE VIRTUAL_ROBOT_INCLUDE_FILES ${VIRTUAL_ROBOT_INCLUDE_DIRECTORY}/*.h)

add_library(${VIRTUAL_ROBOT_LIBRARY_NAME} STATIC
  ${VIRTUAL_ROBOT_SOURCE_FILES}
  ${VIRTUAL_ROBOT_INCLUDE_FILES}
)

# Should be public so when programs link against it include directories are propagated
target_include_directories(${VIRTUAL_ROBOT_LIBRARY_NAME} PUBLIC ${VIRTUAL_ROBOT_INCLUDE_DIRECTORY})

# Should be public so when programs link against it library functions are propagated
# Should eventually remove the scheduler from being a dependency
target_link_libraries(${VIRTUAL_ROBOT_LIBRARY_NAME} PUBLIC ${ROBOT_LIBRARY_NAME} ${SCHEDULER_LIBRARY_NAME})



# Actually decided not to do this
# Should be private so that when also linking against physical_robot, header file includes need to be prefaced with "virtual"
#target_include_directories(${VIRTUAL_ROBOT_LIBRARY_NAME} PRIVATE ${VIRTUAL_ROBOT_INCLUDE_DIRECTORY_NESTED_DIRECTORY})


# # Temporary library while I decide how I want to abstract the core robot features from virtual_robot, physical_robot, common, and test
# add_library(virtual_robot_lib STATIC 
#   ${VIRTUAL_ROBOT_SOURCE_FILES}
#   ${VIRTUAL_ROBOT_INCLUDE_FILES}
# )
# target_include_directories(virtual_robot_lib PUBLIC ${VIRTUAL_ROBOT_SOURCE_DIRECTORY} ${VIRTUAL_ROBOT_INCLUDE_DIRECTORY})
# target_link_libraries(virtual_robot_lib PUBLIC ${SCHEDULER_LIBRARY_NAME})

# add_executable(
#   ${VIRTUAL_ROBOT_EXECUTABLE_NAME}
#   ${VIRTUAL_ROBOT_SOURCE_FILES}
#   ${VIRTUAL_ROBOT_INCLUDE_FILES}
# )

# # Needs to be public so when tests link against it include directories are propagated
# target_include_directories(${VIRTUAL_ROBOT_EXECUTABLE_NAME} PUBLIC ${VIRTUAL_ROBOT_INCLUDE_DIRECTORY})

# # Should be public so when tests link against it the library is propagated
# # Tests technically explicitly link against the library but it is still best practice to be public
# target_link_libraries(${VIRTUAL_ROBOT_EXECUTABLE_NAME} PUBLIC ${SCHEDULER_LIBRARY_NAME} virtual_robot_lib)





# target_link_libraries(${VIRTUAL_ROBOT_TESTS_DIRECTORY} PRIVATE ${SCHEDULER_LIBRARY_NAME} gtest gtest_main)
# add_test(NAME ${SCHEDULER_TESTS_NAME} COMMAND ${SCHEDULER_TESTS_NAME})

# Ensure tests are always built and executed
# if(BUILD_FOR_PHYSICAL)
#     add_custom_target(RunTestsForPhysical
#         COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
#         DEPENDS ${SCHEDULER_LIBRARY_NAME}
#     )
#     add_dependencies(${PHYSICAL_ROBOT_EXECUTABLE_NAME} RunTestsForPhysical)
# endif()