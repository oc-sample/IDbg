set current_dir=%~dp0
call .\pre_commit_windows.bat
cmake -G"Visual Studio 14 2015" %current_dir%\..\CMakeLists.txt -B%current_dir%\..\project\windows
start ..\project\windows\cross_idbg.sln