#!/usr/bin/env bash
export PATH=$CMAKE311_HOME/bin:/usr/bin:$PATH

current_dir=$(cd "$(dirname "$0")";pwd)
echo $current_dir

IOS_PLATFORM=OS
cmake -GXcode -H$current_dir/. -B$current_dir/platforms_project/ios -DIOS_PLATFORM=$IOS_PLATFORM -DCMAKE_TOOLCHAIN_FILE=$current_dir/platforms_project/ios.cmake

echo "QCI_WORKSPACE = $QCI_WORKSPACE"
if [[ -n "${QCI_WORKSPACE}" ]]
then
  echo "base generate simulator project"
  IOS_PLATFORM_SIMULATOR=SIMULATOR
  cmake -GXcode -H$current_dir/. -B$current_dir/platforms_project/ios_simulator -DIOS_PLATFORM=$IOS_PLATFORM_SIMULATOR -DCMAKE_TOOLCHAIN_FILE=$current_dir/platforms_project/ios.cmake
fi
