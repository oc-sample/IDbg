cmake_minimum_required(VERSION 3.10)
message(STATUS "enter top makefile")
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
project(cross_idbg)
add_subdirectory(IDbg)
if (ANDROID)
    message(WARNING "in top cmake ")
    add_subdirectory(platform/android/idbg_demo/app/src/main/cpp)
endif()
#add_subdirectory(app/ios/idbg_demo)