cmake_minimum_required (VERSION 3.16)
project (IDbg VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )
configure_file(version_config.h.in version_config.h)

# Includes
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/base)
include_directories(${PROJECT_SOURCE_DIR}/core)

message(STATUS "project source dir " ${PROJECT_SOURCE_DIR})
message(STATUS "project binary dir " ${PROJECT_BINARY_DIR})
message(STATUS "cmake current dir" ${CMAKE_CURRENT_SOURCE_DIR})

set(TOP_HEADER
        build_config.h
        common_def.h)

if (IOS OR MACOS)
  set(BASE_HEADER
          base/thread_model.h
          base/singleton.h)

  set(BASE_SOURCE
          base/thread_model.cc)

  set(CORE_HEADER
          core/thread_cpu_info.h
          core/mini_dump_file.h
          core/ks_dynamic_linker.h
          core/sys_util.h)

  set(CORE_SOURCE
          core/ks_dynamic_linker.c
          core/mini_dump_file.cc
          core/sys_util.cc
          core/sys_util.mm
          core/thread_cpu_info.cc)

  set(MONITOR_HEADER
          monitor/config_center.h
          monitor/heartbeat.h
          monitor/high_cpu_monitor.h
          monitor/thread_monitor.h)

  set(MONITOR_SOURCE
          monitor/heartbeat.mm
          monitor/high_cpu_monitor.cc
          monitor/thread_monitor.cc)

  set(PUBLIC_HEADER
          core/thread_cpu_info.h
          core/mini_dump_file.h
          base/thread_model.h)

elseif(WIN32 OR ANDROID)
  set(PUBLIC_HEADER
          thread_model.h)

  set (PRIVATE_HEADER
          build_config.h)

  set(SOURCE_FILES
          thread_model.cc)
endif()

# Library
add_library (${PROJECT_NAME}
        ${TOP_HEADER}
        ${BASE_HEADER}
        ${BASE_SOURCE}
        ${CORE_HEADER}
        ${CORE_SOURCE}
        ${MONITOR_HEADER}
        ${MONITOR_SOURCE})

source_group("base/Header Files" FILES ${BASE_HEADER})
source_group("base/Source Files" FILES ${BASE_SOURCE})
source_group("core/Header Files" FILES ${CORE_HEADER})
source_group("core/Source Files" FILES ${CORE_SOURCE})
source_group("monitor/Header Files" FILES ${MONITOR_HEADER})
source_group("monitor/Source Files" FILES ${MONITOR_SOURCE})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_SHARED_LIBS)
  target_compile_definitions(${PROJECT_NAME} PUBLIC IS_BUILDING_SHARED)
  message(STATUS "Building shared version...")
else()
  message(STATUS "Building static version...")
endif()

if (IOS)
  target_link_libraries(${PROJECT_NAME} "-framework Foundation -framework UIKit")
  message(STATUS "build ios poroject")
  set_target_properties(${PROJECT_NAME} PROPERTIES FRAMEWORK TRUE
          PUBLIC_HEADER "${PUBLIC_HEADER}"
          XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
          XCODE_ATTRIBUTE_SKIP_INSTALL "YES"
          #ios.toolchain.cmake haven't this abilityg
          XCODE_ATTRIBUTE_EXCLUDED_ARCHS[sdk=iphonesimulator*] "arm64"
          XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] "YES"
          MACOSX_FRAMEWORK_IDENTIFIER "com.mj.${PROJECT_NAME}"
          XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES"
          XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
          #XCODE_ATTRIBUTE_PROVISIONING_PROFILE_SPECIFIER "ccfd1595-ba25-4cdd-bcb5-88848a6ea773"
          #XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
          )

  set(PROJECT_AUTO ${PROJECT_NAME}-auto)
  ADD_CUSTOM_TARGET(${PROJECT_AUTO})
  ADD_CUSTOM_COMMAND(TARGET ${PROJECT_AUTO} COMMAND /bin/bash /${CMAKE_SOURCE_DIR}/tools/merge_frame.sh)
ELSE(OSX)
  message(STATUS "build OSX poroject")
  set_target_properties(${PROJECT_NAME} PROPERTIES FRAMEWORK TRUE
          PUBLIC_HEADER "${PUBLIC_HEADER}"
          XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
          XCODE_ATTRIBUTE_SKIP_INSTALL "YES"
          #ios.toolchain.cmake haven't this abilityg
          #XCODE_ATTRIBUTE_EXCLUDED_ARCHS[sdk=iphonesimulator*] "arm64"
          XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] "YES"
          MACOSX_FRAMEWORK_IDENTIFIER "com.mj.${PROJECT_NAME}"
          XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES"
          XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
          )
endif()

#if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#  if (IOS)
#    set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../platform/ios CACHE PATH "Install path" FORCE)
#  else(MACOS)
#    set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../platform/osx CACHE PATH "Install path" FORCE)
#  endif ()
#endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#
#message(STATUS "install prefix .." ${CMAKE_INSTALL_PREFIX})
#
#install(TARGETS ${PROJECT_NAME}
#        LIBRARY DESTINATION lib
#        FRAMEWORK DESTINATION framework
#        ARCHIVE DESTINATION lib/static)
#
#install (FILES ${PUBLIC_HEADER} DESTINATION include)
