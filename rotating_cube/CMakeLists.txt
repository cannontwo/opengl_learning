set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_executable(rotating_cube rotating_cube.cpp InitShader.cpp)
target_link_libraries(rotating_cube
  ${LIBRARIES}
  )
