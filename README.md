# 配置生成静态库--工程默认生成动态库
方法1. 编译脚本（ios:generate_ios_proj.sh）添加 -DBUILD_SHARED_LIBS=0

方法2. 直接修改顶层CMakeList.txt option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# ios平台构建过程
1. cd tools
2. ./generate_ios_proj.sh
3. 打开project/ios/cross_idbg.xcodeproj, 编译IDbg-auto
4. 打开platform/ios/idbg_demo.xcodeproj 编译idgb_demo

# mac 平台构建
1. cd tools
2. ./generate_osx_proj.sh
3. 打开project/osx/cross_idbg.xcodeproj, 编译IDbg
4. 打开platform/osx/idbg_demo.xcodeproj 编译idgb_demo

# windows 平台构建
1. cd tools
2. .\generate_vs_proj.bat
3. 打开project/windows/cross_idbg.sln, 编译idbg
4. 打开platform/windows/idbg_demo.sln, 编译idgb_demo
