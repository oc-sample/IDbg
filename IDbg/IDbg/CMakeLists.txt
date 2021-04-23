cmake_minimum_required(VERSION 3.16)
project(IDbg)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_library(IDbg STATIC
        IDbg.h
        ks_dynamic_linker.c
        ks_dynamic_linker.h
        mini_dump_file.cc
        mini_dump_file.h
        sys_util.h
        sys_util.mm
        thread_cpu_info.cc
        thread_cpu_info.h
        thread_model.cc
        thread_model.h)
        
set(HEADER_FILES
mini_dump_file.h
thread_model.h
thread_cpu_info.h
)
        
set_target_properties(IDbg PROPERTIES FRAMEWORK TRUE
PUBLIC_HEADER "${HEADER_FILES}"
)
