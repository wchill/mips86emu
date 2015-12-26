cmake_minimum_required(VERSION 3.3)
project(mips86core)
include_directories("include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
add_executable(mips86core ${SOURCE_FILES})