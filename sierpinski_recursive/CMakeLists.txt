set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_executable(sierpinski_recursive sierpinski_recursive.cpp InitShader.cpp)
target_link_libraries(sierpinski_recursive
  ${LIBRARIES}
  )
