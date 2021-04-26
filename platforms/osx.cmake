# ﻿cmake_minimum_required(VERSION 3.4.1)

set (CMAKE_SYSTEM_NAME Darwin)
set (UNIX 1)
set (APPLE 1)
set (OSX 1)

macro (find_osx_framework VAR fwname)
    find_library(FRAMEWORK_${fwname}
                 NAMES ${fwname}
                 PATHS ${CMAKE_OSX_SYSROOT}/System/Library
                 PATH_SUFFIXES Frameworks
                 NO_DEFAULT_PATH)

    if(${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        message(ERROR "Framework ${fwname} not found")
    else()
        message(STATUS "Framwork ${fwname} found at ${FRAMEWORK_${fwname}}")
        set(${VAR} ${FRAMEWORK_${fwname}})
    endif()
endmacro(find_osx_framework)

macro (set_xcode_attr_property TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endmacro (set_xcode_attr_property)

macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property (TARGET ${TARGET} PROPERTY ${XCODE_PROPERTY} ${XCODE_VALUE})
endmacro (set_xcode_property)
