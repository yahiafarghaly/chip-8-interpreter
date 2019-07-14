cmake_minimum_required(VERSION 2.8.4)
project(CHIP_8_Interpreter)

SET(execName chip8)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread") # For std::thread


set(SOURCE_FILES main.cc Chip_8.cc chip_8_opcode_functions.cc)
add_executable(${execName} ${SOURCE_FILES})

target_link_libraries(${execName} GL GLU glut)