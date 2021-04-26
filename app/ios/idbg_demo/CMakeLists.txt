cmake_minimum_required(VERSION 3.16)

project(idbg_demo)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

file(GLOB
     idbg_source
     "*.h"
     "*.m"
     "*.mm"
     "*.c"
     "*.cc")

add_executable(idbg_demo ${idbg_source})