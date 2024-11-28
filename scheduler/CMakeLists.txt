set(SCHEDULER_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SCHEDULER_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SCHEDULER_SOURCE_FILES ${SCHEDULER_SOURCE_DIRECTORY}/*.cc)
file(GLOB_RECURSE SCHEDULER_INCLUDE_FILES ${SCHEDULER_INCLUDE_DIRECTORY}/*.h)

add_library(${SCHEDULER_LIBRARY_NAME} STATIC
  ${SCHEDULER_SOURCE_FILES}
  ${SCHEDULER_INCLUDE_FILES}
)

# Should be public so when programs link against it include directories are propagated
# I am including source directories as well for generic classes that don't have header files
target_include_directories(${SCHEDULER_LIBRARY_NAME} PUBLIC ${SCHEDULER_SOURCE_DIRECTORY} ${SCHEDULER_INCLUDE_DIRECTORY})