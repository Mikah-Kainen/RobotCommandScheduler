add_executable(${MAIN_EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PUBLIC ${SCHEDULER_LIBRARY_NAME} ${ROBOT_LIBRARY_NAME} ${VIRTUAL_ROBOT_LIBRARY_NAME} ${PHYSICAL_ROBOT_LIBRARY_NAME})

# Changing from default of build/main/ to make it easier to find for new users
set_target_properties(${MAIN_EXECUTABLE_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)