#!/bin/bash

echo "hello"
#env


# Type a script or drag a script file from your workspace to insert its path.
if [ "${ACTION}" = "build" ]
then

echo ${ACTION}

#编译模式  ${CONFIGURATION}   Release   Debug

#要build的target名
target_Name=${PROJECT_NAME}
#build之后的文件夹路径
#build_DIR=${SRCROOT}/build
#build_DIR=${BUILD_DIR}
#echo ${build_DIR}

echo ${build_DIR}
#真机build生成的framework文件路径
DEVICE_DIR_Framework=${BUILD_DIR}/${CONFIGURATION}-iphoneos/${PROJECT_NAME}.framework
#模拟器build生成的framework文件路径
SIMULATOR_DIR_Framework=${BUILD_DIR}/${CONFIGURATION}-iphonesimulator/${PROJECT_NAME}.framework
#目标文件夹路径
INSTALL_DIR=${SRCROOT}/Products/${PROJECT_NAME}/${CONFIGURATION}
#判断build文件夹是否存在，存在则删除
if [ -d "${build_DIR}" ]
then
rm -rf "${build_DIR}"
fi
#判断目标文件夹是否存在，存在则删除该文件夹
if [ -d "${INSTALL_DIR}" ]
then
rm -rf "${INSTALL_DIR}"
fi
#创建目标文件夹
mkdir -p "${INSTALL_DIR}"
pwd
cd ../
#build之前clean一下
#xcodebuild -target ${target_Name} clean
#真机build
xcodebuild -target ${target_Name} ONLY_ACTIVE_ARCH=NO -configuration ${CONFIGURATION} -sdk iphoneos
#模拟器build
xcodebuild -target ${target_Name} ONLY_ACTIVE_ARCH=NO -configuration ${CONFIGURATION} -sdk iphonesimulator
#复制头文件到目标文件夹
cp -R "${DEVICE_DIR_Framework}" "${INSTALL_DIR}"
#合成模拟器和真机包
lipo -create "${DEVICE_DIR_Framework}/${PROJECT_NAME}" "${SIMULATOR_DIR_Framework}/${PROJECT_NAME}" -output "${INSTALL_DIR}/${PROJECT_NAME}.framework/${PROJECT_NAME}"
#打开目标文件夹
open "${INSTALL_DIR}"

fi

