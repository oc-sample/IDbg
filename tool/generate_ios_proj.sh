#!/bin/bash

current_dir=$(cd "$(dirname "$0")";pwd)

#echo ${current_dir}

#project_name=$(basename ${current_dir})
#echo ${project_name}

cmake .. -GXcode -B $current_dir/../project/ios -DCMAKE_TOOLCHAIN_FILE=$current_dir/ios.toolchain.cmake -DPLATFORM=OS64COMBINED -DARCHS="arm64 armv7 armv7s i386 x86_64" -DDEPLOYMENT_TARGET=9.0
