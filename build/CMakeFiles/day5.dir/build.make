# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.26.4/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.26.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/vilmer/Documents/Projects/AdventOfCode2023

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/vilmer/Documents/Projects/AdventOfCode2023/build

# Include any dependencies generated for this target.
include CMakeFiles/day5.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/day5.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/day5.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/day5.dir/flags.make

CMakeFiles/day5.dir/src/5/main.cc.o: CMakeFiles/day5.dir/flags.make
CMakeFiles/day5.dir/src/5/main.cc.o: /Users/vilmer/Documents/Projects/AdventOfCode2023/src/5/main.cc
CMakeFiles/day5.dir/src/5/main.cc.o: CMakeFiles/day5.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vilmer/Documents/Projects/AdventOfCode2023/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/day5.dir/src/5/main.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/day5.dir/src/5/main.cc.o -MF CMakeFiles/day5.dir/src/5/main.cc.o.d -o CMakeFiles/day5.dir/src/5/main.cc.o -c /Users/vilmer/Documents/Projects/AdventOfCode2023/src/5/main.cc

CMakeFiles/day5.dir/src/5/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/day5.dir/src/5/main.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vilmer/Documents/Projects/AdventOfCode2023/src/5/main.cc > CMakeFiles/day5.dir/src/5/main.cc.i

CMakeFiles/day5.dir/src/5/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/day5.dir/src/5/main.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vilmer/Documents/Projects/AdventOfCode2023/src/5/main.cc -o CMakeFiles/day5.dir/src/5/main.cc.s

# Object files for target day5
day5_OBJECTS = \
"CMakeFiles/day5.dir/src/5/main.cc.o"

# External object files for target day5
day5_EXTERNAL_OBJECTS =

day5: CMakeFiles/day5.dir/src/5/main.cc.o
day5: CMakeFiles/day5.dir/build.make
day5: libCCLib.a
day5: CMakeFiles/day5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vilmer/Documents/Projects/AdventOfCode2023/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable day5"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/day5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/day5.dir/build: day5
.PHONY : CMakeFiles/day5.dir/build

CMakeFiles/day5.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/day5.dir/cmake_clean.cmake
.PHONY : CMakeFiles/day5.dir/clean

CMakeFiles/day5.dir/depend:
	cd /Users/vilmer/Documents/Projects/AdventOfCode2023/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vilmer/Documents/Projects/AdventOfCode2023 /Users/vilmer/Documents/Projects/AdventOfCode2023 /Users/vilmer/Documents/Projects/AdventOfCode2023/build /Users/vilmer/Documents/Projects/AdventOfCode2023/build /Users/vilmer/Documents/Projects/AdventOfCode2023/build/CMakeFiles/day5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/day5.dir/depend

