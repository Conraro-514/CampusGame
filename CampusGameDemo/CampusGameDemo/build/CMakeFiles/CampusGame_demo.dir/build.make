# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/momo/Desktop/CampusGameDemo/CampusGameDemo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/momo/Desktop/CampusGameDemo/CampusGameDemo/build

# Include any dependencies generated for this target.
include CMakeFiles/CampusGame_demo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CampusGame_demo.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CampusGame_demo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CampusGame_demo.dir/flags.make

CMakeFiles/CampusGame_demo.dir/main.cpp.o: CMakeFiles/CampusGame_demo.dir/flags.make
CMakeFiles/CampusGame_demo.dir/main.cpp.o: /home/momo/Desktop/CampusGameDemo/CampusGameDemo/main.cpp
CMakeFiles/CampusGame_demo.dir/main.cpp.o: CMakeFiles/CampusGame_demo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/momo/Desktop/CampusGameDemo/CampusGameDemo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CampusGame_demo.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CampusGame_demo.dir/main.cpp.o -MF CMakeFiles/CampusGame_demo.dir/main.cpp.o.d -o CMakeFiles/CampusGame_demo.dir/main.cpp.o -c /home/momo/Desktop/CampusGameDemo/CampusGameDemo/main.cpp

CMakeFiles/CampusGame_demo.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CampusGame_demo.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/momo/Desktop/CampusGameDemo/CampusGameDemo/main.cpp > CMakeFiles/CampusGame_demo.dir/main.cpp.i

CMakeFiles/CampusGame_demo.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CampusGame_demo.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/momo/Desktop/CampusGameDemo/CampusGameDemo/main.cpp -o CMakeFiles/CampusGame_demo.dir/main.cpp.s

# Object files for target CampusGame_demo
CampusGame_demo_OBJECTS = \
"CMakeFiles/CampusGame_demo.dir/main.cpp.o"

# External object files for target CampusGame_demo
CampusGame_demo_EXTERNAL_OBJECTS =

CampusGame_demo: CMakeFiles/CampusGame_demo.dir/main.cpp.o
CampusGame_demo: CMakeFiles/CampusGame_demo.dir/build.make
CampusGame_demo: /usr/local/lib/libopencv_gapi.so.4.5.5
CampusGame_demo: /usr/lib/libCampusGame.a
CampusGame_demo: /usr/local/lib/libopencv_highgui.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_ml.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_objdetect.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_photo.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_stitching.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_video.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_calib3d.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_dnn.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_features2d.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_flann.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_videoio.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_imgcodecs.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_imgproc.so.4.5.5
CampusGame_demo: /usr/local/lib/libopencv_core.so.4.5.5
CampusGame_demo: /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1.7.4
CampusGame_demo: CMakeFiles/CampusGame_demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/momo/Desktop/CampusGameDemo/CampusGameDemo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CampusGame_demo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CampusGame_demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CampusGame_demo.dir/build: CampusGame_demo
.PHONY : CMakeFiles/CampusGame_demo.dir/build

CMakeFiles/CampusGame_demo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CampusGame_demo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CampusGame_demo.dir/clean

CMakeFiles/CampusGame_demo.dir/depend:
	cd /home/momo/Desktop/CampusGameDemo/CampusGameDemo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/momo/Desktop/CampusGameDemo/CampusGameDemo /home/momo/Desktop/CampusGameDemo/CampusGameDemo /home/momo/Desktop/CampusGameDemo/CampusGameDemo/build /home/momo/Desktop/CampusGameDemo/CampusGameDemo/build /home/momo/Desktop/CampusGameDemo/CampusGameDemo/build/CMakeFiles/CampusGame_demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CampusGame_demo.dir/depend

