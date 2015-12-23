cmake_minimum_required(VERSION 3.3)
project(mips86core)
include_directories("include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
add_executable(mips86core ${SOURCE_FILES})