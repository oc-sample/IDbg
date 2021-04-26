#!/usr/bin/env bash
export PATH=$CMAKE311_HOME/bin:/usr/bin:$PATH

IOS_PLATFORM=OS

current_dir=$(cd "$(dirname "$0")";pwd)
echo $current_dir

cmake -GXcode -H$current_dir/. -B$current_dir/platforms/osx -DIOS_PLATFORM=$IOS_PLATFORM -DCMAKE_TOOLCHAIN_FILE=$current_dir/platforms/osx.cmake -DCMAKE_OSX_ARCHITECTURES=x86_64
