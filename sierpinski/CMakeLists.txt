set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_executable(sierpinski sierpinski.cpp InitShader.cpp)
target_link_libraries(sierpinski
  ${LIBRARIES}
  )
