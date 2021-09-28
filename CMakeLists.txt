cmake_minimum_required(VERSION 3.0)

project(naiveMAPF)

include_directories(lib, src, test)

set(EXECUTABLE_OUTPUT_PATH bin)
set(CMAKE_CXX_STANDARD 11)
add_executable(testCBS ${PROJECT_SOURCE_DIR}/test/testCBS.cpp)
#add_executable(test2json ${PROJECT_SOURCE_DIR}/test/test2json.cpp)
