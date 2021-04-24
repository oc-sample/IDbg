set current_dir=%~dp0
cmake -G"Visual Studio 14 2015" %current_dir%CMakeLists.txt -B%current_dir%platforms_project/win32
