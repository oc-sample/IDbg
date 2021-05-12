#!/bin/bash

current_dir=$(cd "$(dirname "$0")";pwd)

#echo ${current_dir}

#project_name=$(basename ${current_dir})
#echo ${project_name}

cmake .. -GXcode -B $current_dir/../project/osx -DCMAKE_TOOLCHAIN_FILE=$current_dir/ios.toolchain.cmake -DPLATFORM=MAC -DARCHS="arm64 x86_64" -DENABLE_BITCODE=0

open ../project/osx/cross_idbg.xcodeproj/
