# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/tidesun/repos/BFBGraph

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/tidesun/repos/BFBGraph/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/BFBDetectionGraph.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/BFBDetectionGraph.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BFBDetectionGraph.dir/flags.make

CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.o: CMakeFiles/BFBDetectionGraph.dir/flags.make
CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.o: ../src/bfb_algorithm.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tidesun/repos/BFBGraph/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.o -c /Users/tidesun/repos/BFBGraph/src/bfb_algorithm.cpp

CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tidesun/repos/BFBGraph/src/bfb_algorithm.cpp > CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.i

CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tidesun/repos/BFBGraph/src/bfb_algorithm.cpp -o CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.s

CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.o: CMakeFiles/BFBDetectionGraph.dir/flags.make
CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tidesun/repos/BFBGraph/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.o -c /Users/tidesun/repos/BFBGraph/src/main.cpp

CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tidesun/repos/BFBGraph/src/main.cpp > CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.i

CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tidesun/repos/BFBGraph/src/main.cpp -o CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.s

CMakeFiles/BFBDetectionGraph.dir/main.cpp.o: CMakeFiles/BFBDetectionGraph.dir/flags.make
CMakeFiles/BFBDetectionGraph.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tidesun/repos/BFBGraph/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/BFBDetectionGraph.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BFBDetectionGraph.dir/main.cpp.o -c /Users/tidesun/repos/BFBGraph/main.cpp

CMakeFiles/BFBDetectionGraph.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BFBDetectionGraph.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tidesun/repos/BFBGraph/main.cpp > CMakeFiles/BFBDetectionGraph.dir/main.cpp.i

CMakeFiles/BFBDetectionGraph.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BFBDetectionGraph.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tidesun/repos/BFBGraph/main.cpp -o CMakeFiles/BFBDetectionGraph.dir/main.cpp.s

# Object files for target BFBDetectionGraph
BFBDetectionGraph_OBJECTS = \
"CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.o" \
"CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.o" \
"CMakeFiles/BFBDetectionGraph.dir/main.cpp.o"

# External object files for target BFBDetectionGraph
BFBDetectionGraph_EXTERNAL_OBJECTS =

BFBDetectionGraph: CMakeFiles/BFBDetectionGraph.dir/src/bfb_algorithm.cpp.o
BFBDetectionGraph: CMakeFiles/BFBDetectionGraph.dir/src/main.cpp.o
BFBDetectionGraph: CMakeFiles/BFBDetectionGraph.dir/main.cpp.o
BFBDetectionGraph: CMakeFiles/BFBDetectionGraph.dir/build.make
BFBDetectionGraph: CMakeFiles/BFBDetectionGraph.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/tidesun/repos/BFBGraph/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable BFBDetectionGraph"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BFBDetectionGraph.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BFBDetectionGraph.dir/build: BFBDetectionGraph

.PHONY : CMakeFiles/BFBDetectionGraph.dir/build

CMakeFiles/BFBDetectionGraph.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BFBDetectionGraph.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BFBDetectionGraph.dir/clean

CMakeFiles/BFBDetectionGraph.dir/depend:
	cd /Users/tidesun/repos/BFBGraph/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/tidesun/repos/BFBGraph /Users/tidesun/repos/BFBGraph /Users/tidesun/repos/BFBGraph/cmake-build-debug /Users/tidesun/repos/BFBGraph/cmake-build-debug /Users/tidesun/repos/BFBGraph/cmake-build-debug/CMakeFiles/BFBDetectionGraph.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BFBDetectionGraph.dir/depend

