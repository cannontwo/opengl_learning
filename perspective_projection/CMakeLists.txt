set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_executable(perspective_projection perspective_projection.cpp InitShader.cpp)
target_link_libraries(perspective_projection
  ${LIBRARIES}
  )
