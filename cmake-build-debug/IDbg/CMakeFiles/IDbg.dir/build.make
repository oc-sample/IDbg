# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mjzheng/Documents/mj_git/oc-sample/IDbg

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug

# Include any dependencies generated for this target.
include IDbg/CMakeFiles/IDbg.dir/depend.make

# Include the progress variables for this target.
include IDbg/CMakeFiles/IDbg.dir/progress.make

# Include the compile flags for this target's objects.
include IDbg/CMakeFiles/IDbg.dir/flags.make

IDbg/IDbg.framework/Versions/A/Headers/thread_model.h: ../IDbg/thread_model.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Copying OS X content IDbg/IDbg.framework/Versions/A/Headers/thread_model.h"
	$(CMAKE_COMMAND) -E copy /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/thread_model.h IDbg/IDbg.framework/Versions/A/Headers/thread_model.h

IDbg/CMakeFiles/IDbg.dir/ks_dynamic_linker.c.o: IDbg/CMakeFiles/IDbg.dir/flags.make
IDbg/CMakeFiles/IDbg.dir/ks_dynamic_linker.c.o: ../IDbg/ks_dynamic_linker.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object IDbg/CMakeFiles/IDbg.dir/ks_dynamic_linker.c.o"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/IDbg.dir/ks_dynamic_linker.c.o -c /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/ks_dynamic_linker.c

IDbg/CMakeFiles/IDbg.dir/ks_dynamic_linker.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/IDbg.dir/ks_dynamic_linker.c.i"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/ks_dynamic_linker.c > CMakeFiles/IDbg.dir/ks_dynamic_linker.c.i

IDbg/CMakeFiles/IDbg.dir/ks_dynamic_linker.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/IDbg.dir/ks_dynamic_linker.c.s"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/ks_dynamic_linker.c -o CMakeFiles/IDbg.dir/ks_dynamic_linker.c.s

IDbg/CMakeFiles/IDbg.dir/mini_dump_file.cc.o: IDbg/CMakeFiles/IDbg.dir/flags.make
IDbg/CMakeFiles/IDbg.dir/mini_dump_file.cc.o: ../IDbg/mini_dump_file.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object IDbg/CMakeFiles/IDbg.dir/mini_dump_file.cc.o"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/IDbg.dir/mini_dump_file.cc.o -c /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/mini_dump_file.cc

IDbg/CMakeFiles/IDbg.dir/mini_dump_file.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IDbg.dir/mini_dump_file.cc.i"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/mini_dump_file.cc > CMakeFiles/IDbg.dir/mini_dump_file.cc.i

IDbg/CMakeFiles/IDbg.dir/mini_dump_file.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IDbg.dir/mini_dump_file.cc.s"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/mini_dump_file.cc -o CMakeFiles/IDbg.dir/mini_dump_file.cc.s

IDbg/CMakeFiles/IDbg.dir/sys_util.cc.o: IDbg/CMakeFiles/IDbg.dir/flags.make
IDbg/CMakeFiles/IDbg.dir/sys_util.cc.o: ../IDbg/sys_util.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object IDbg/CMakeFiles/IDbg.dir/sys_util.cc.o"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/IDbg.dir/sys_util.cc.o -c /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/sys_util.cc

IDbg/CMakeFiles/IDbg.dir/sys_util.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IDbg.dir/sys_util.cc.i"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/sys_util.cc > CMakeFiles/IDbg.dir/sys_util.cc.i

IDbg/CMakeFiles/IDbg.dir/sys_util.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IDbg.dir/sys_util.cc.s"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/sys_util.cc -o CMakeFiles/IDbg.dir/sys_util.cc.s

IDbg/CMakeFiles/IDbg.dir/thread_cpu_info.cc.o: IDbg/CMakeFiles/IDbg.dir/flags.make
IDbg/CMakeFiles/IDbg.dir/thread_cpu_info.cc.o: ../IDbg/thread_cpu_info.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object IDbg/CMakeFiles/IDbg.dir/thread_cpu_info.cc.o"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/IDbg.dir/thread_cpu_info.cc.o -c /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/thread_cpu_info.cc

IDbg/CMakeFiles/IDbg.dir/thread_cpu_info.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IDbg.dir/thread_cpu_info.cc.i"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/thread_cpu_info.cc > CMakeFiles/IDbg.dir/thread_cpu_info.cc.i

IDbg/CMakeFiles/IDbg.dir/thread_cpu_info.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IDbg.dir/thread_cpu_info.cc.s"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/thread_cpu_info.cc -o CMakeFiles/IDbg.dir/thread_cpu_info.cc.s

IDbg/CMakeFiles/IDbg.dir/thread_model.cc.o: IDbg/CMakeFiles/IDbg.dir/flags.make
IDbg/CMakeFiles/IDbg.dir/thread_model.cc.o: ../IDbg/thread_model.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object IDbg/CMakeFiles/IDbg.dir/thread_model.cc.o"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/IDbg.dir/thread_model.cc.o -c /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/thread_model.cc

IDbg/CMakeFiles/IDbg.dir/thread_model.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IDbg.dir/thread_model.cc.i"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/thread_model.cc > CMakeFiles/IDbg.dir/thread_model.cc.i

IDbg/CMakeFiles/IDbg.dir/thread_model.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IDbg.dir/thread_model.cc.s"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg/thread_model.cc -o CMakeFiles/IDbg.dir/thread_model.cc.s

# Object files for target IDbg
IDbg_OBJECTS = \
"CMakeFiles/IDbg.dir/ks_dynamic_linker.c.o" \
"CMakeFiles/IDbg.dir/mini_dump_file.cc.o" \
"CMakeFiles/IDbg.dir/sys_util.cc.o" \
"CMakeFiles/IDbg.dir/thread_cpu_info.cc.o" \
"CMakeFiles/IDbg.dir/thread_model.cc.o"

# External object files for target IDbg
IDbg_EXTERNAL_OBJECTS =

IDbg/IDbg.framework/Versions/A/IDbg: IDbg/CMakeFiles/IDbg.dir/ks_dynamic_linker.c.o
IDbg/IDbg.framework/Versions/A/IDbg: IDbg/CMakeFiles/IDbg.dir/mini_dump_file.cc.o
IDbg/IDbg.framework/Versions/A/IDbg: IDbg/CMakeFiles/IDbg.dir/sys_util.cc.o
IDbg/IDbg.framework/Versions/A/IDbg: IDbg/CMakeFiles/IDbg.dir/thread_cpu_info.cc.o
IDbg/IDbg.framework/Versions/A/IDbg: IDbg/CMakeFiles/IDbg.dir/thread_model.cc.o
IDbg/IDbg.framework/Versions/A/IDbg: IDbg/CMakeFiles/IDbg.dir/build.make
IDbg/IDbg.framework/Versions/A/IDbg: IDbg/CMakeFiles/IDbg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX shared library IDbg.framework/IDbg"
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/IDbg.dir/link.txt --verbose=$(VERBOSE)

IDbg/IDbg.framework/IDbg: IDbg/IDbg.framework/Versions/A/IDbg
	@$(CMAKE_COMMAND) -E touch_nocreate IDbg/IDbg.framework/IDbg

# Rule to build all files generated by this target.
IDbg/CMakeFiles/IDbg.dir/build: IDbg/IDbg.framework/IDbg
IDbg/CMakeFiles/IDbg.dir/build: IDbg/IDbg.framework/Versions/A/Headers/thread_model.h

.PHONY : IDbg/CMakeFiles/IDbg.dir/build

IDbg/CMakeFiles/IDbg.dir/clean:
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg && $(CMAKE_COMMAND) -P CMakeFiles/IDbg.dir/cmake_clean.cmake
.PHONY : IDbg/CMakeFiles/IDbg.dir/clean

IDbg/CMakeFiles/IDbg.dir/depend:
	cd /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mjzheng/Documents/mj_git/oc-sample/IDbg /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/IDbg /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/cmake-build-debug/IDbg/CMakeFiles/IDbg.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : IDbg/CMakeFiles/IDbg.dir/depend
