cmake_minimum_required(VERSION 3.16)

project(IDbg)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

file(GLOB
     idbg_source
     "*.h"
     "*.cc"
     "*.c")
     
set(HEADER_FILES
thread_model.h
)
     
if (IOS OR OSX)
list(APPEND idbg_source sys_util.mm)
list(APPEND HEADER_FILES thread_cpu_info.h mini_dump_file.h)
elseif(WIN32 OR ANDROID)
  list(FILTER idbg_source EXCLUDE REGEX "thread_cpu_info.cc")
  list(FILTER idbg_source EXCLUDE REGEX "thread_cpu_info.h")
  list(FILTER idbg_source EXCLUDE REGEX "mini_dump_file.cc")
  list(FILTER idbg_source EXCLUDE REGEX "mini_dump_file.h")
  list(FILTER idbg_source EXCLUDE REGEX "ks_dynamic_linker.c")
  list(FILTER idbg_source EXCLUDE REGEX "ks_dynamic_linker.h")
  list(FILTER idbg_source EXCLUDE REGEX "sys_util.h")
endif()
     
add_library(IDbg STATIC ${idbg_source})
        
        
set_target_properties(IDbg PROPERTIES FRAMEWORK TRUE
PUBLIC_HEADER "${HEADER_FILES}"
XCODE_ATTRIBUTE_SKIP_INSTALL "YES"
)

if (IOS)
ADD_CUSTOM_TARGET(IDbgAuto
)
ADD_CUSTOM_COMMAND(
          TARGET IDbgAuto
          COMMAND /bin/bash /${CMAKE_SOURCE_DIR}/merge_frame.sh
)
endif()
