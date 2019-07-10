# Project configuration details
cmake_minimum_required(VERSION 3.5)
project(opengl_learning)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
list(APPEND LIBRARIES ${OPENGL_LIBRARIES})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
list(APPEND LIBRARIES ${GLUT_LIBRARY})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
list(APPEND LIBRARIES ${GLEW_LIBRARIES})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(sierpinski)
add_subdirectory(sierpinski_recursive)
