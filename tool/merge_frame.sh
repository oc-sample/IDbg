#!/bin/bash
env
# Type a script or drag a script file from your workspace to insert its path.
if [ "${ACTION}" = "build" ]
then

#echo ${ACTION}
#编译模式  ${CONFIGURATION}   Release   Debug

#要build的target名
LIBRARY_NAME=$(basename ${BUILD_ROOT})
echo "libaray name" ${LIBRARY_NAME}

#真机build生成的framework文件路径
DEVICE_Framework=${BUILD_DIR}/${CONFIGURATION}-iphoneos/${LIBRARY_NAME}.framework
echo "divice dir" ${DEVICE_Framework}
#模拟器build生成的framework文件路径
SIMULATOR_Framework=${BUILD_DIR}/${CONFIGURATION}-iphonesimulator/${LIBRARY_NAME}.framework

#目标文件夹路径
PRODUCT_DIR=${BUILD_DIR}/product/${CONFIGURATION}

#判断目标文件夹是否存在，存在则删除该文件夹
if [ -d "${PRODUCT_DIR}" ]
then
echo "remove install dir"
rm -rf "${PRODUCT_DIR}"
fi
#创建目标文件夹
mkdir -p "${PRODUCT_DIR}"
pwd

cd ../
#build之前clean一下
xcodebuild -target ${LIBRARY_NAME} clean

#真机build
xcodebuild -target ${LIBRARY_NAME} ONLY_ACTIVE_ARCH=NO -configuration ${CONFIGURATION} -sdk iphoneos
#模拟器build
xcodebuild -target ${LIBRARY_NAME} ONLY_ACTIVE_ARCH=NO -configuration ${CONFIGURATION} -sdk iphonesimulator
#复制头文件到目标文件夹
cp -R "${DEVICE_Framework}" "${PRODUCT_DIR}"
#合成模拟器和真机包
lipo -create "${DEVICE_Framework}/${LIBRARY_NAME}" "${SIMULATOR_Framework}/${LIBRARY_NAME}" -output "${PRODUCT_DIR}/${LIBRARY_NAME}.framework/${LIBRARY_NAME}"
#打开目标文件夹
open "${PRODUCT_DIR}"
fi

