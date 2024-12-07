
if(BUILD_PHYSICAL)

  set(PHYSICAL_BUILD_MAIN gmr_main.cc)
  set(DOCKER_IMAGE "ev3cc")
  set(CROSS_COMPILER "arm-linux-gnueabi-g++")
  set(EV3_LIB "gmr_libev3dev.a")

  # find_program(DOCKER_EXECUTABLE docker)
  # if(NOT DOCKER_EXECUTABLE)
  #     message(FATAL_ERROR "Docker not found. Please install Docker to build for the physical robot.")
  # endif()

  # arm-linux-gnueabi-g++ -o /src/main_build main/src/gmr_main.cc build/scheduler/libscheduler_binary.a build/robot_library/librobot_library_binary.a build/physical_robot/libphysical_robot_library_binary.a lib/gmr_libev3dev.a
  # docker run --rm -it -v /home/mikah/RobotCommandScheduler/:/src -w /src ev3cc
  message(STATUS "Command to be executed: ${CROSS_COMPILER} -o /src/${MAIN_EXECUTABLE_NAME}
        main/src/${PHYSICAL_BUILD_MAIN}
        build/scheduler/lib${SCHEDULER_LIBRARY_NAME}.a
        build/robot_library/lib${ROBOT_LIBRARY_NAME}.a
        build/physical_robot/lib${PHYSICAL_ROBOT_LIBRARY_NAME}.a
        lib/${EV3_LIB}")

  # # Add custom command to build in Docker
  # add_custom_target(
  #   physical_robot_build
  #   #COMMAND docker run --rm -it --log-driver=none
  #   COMMAND docker run --rm -d 
  #     #-v ${CMAKE_SOURCE_DIR}/:/src 
  #     -v /home/mikah/RobotCommandScheduler/:/src
  #     -w /src 
  #     ${DOCKER_IMAGE} 
  #     ${CROSS_COMPILER} -o /src/${MAIN_EXECUTABLE_NAME}
  #       main/src/${PHYSICAL_BUILD_MAIN}
  #       build/scheduler/lib${SCHEDULER_LIBRARY_NAME}.a
  #       build/robot_library/lib${ROBOT_LIBRARY_NAME}.a
  #       build/physical_robot/lib${PHYSICAL_ROBOT_LIBRARY_NAME}.a
  #       lib/${EV3_LIB}
  #   COMMAND docker logs $(docker ps -ql)
  #   COMMENT "Building the physical robot binary using Docker"
  # )

  add_executable(${MAIN_EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/${PHYSICAL_BUILD_MAIN})
  target_link_libraries(${MAIN_EXECUTABLE_NAME} PUBLIC ${SCHEDULER_LIBRARY_NAME} ${ROBOT_LIBRARY_NAME} ${PHYSICAL_ROBOT_LIBRARY_NAME})
else()
  add_executable(${MAIN_EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc)
  target_link_libraries(${MAIN_EXECUTABLE_NAME} PUBLIC ${SCHEDULER_LIBRARY_NAME} ${ROBOT_LIBRARY_NAME} ${VIRTUAL_ROBOT_LIBRARY_NAME})
endif()


# # Changing from default of build/main/ to make it easier to find for new users
# set_target_properties(${MAIN_EXECUTABLE_NAME} PROPERTIES
#   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
# )