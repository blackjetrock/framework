# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/menadue/pico/framework/framework_16/macropad/v2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/menadue/pico/framework/framework_16/macropad/v2/build

# Utility rule file for picotoolForceReconfigure.

# Include any custom commands dependencies for this target.
include pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure.dir/compiler_depend.make

# Include the progress variables for this target.
include pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure.dir/progress.make

pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure:
	cd /home/menadue/pico/framework/framework_16/macropad/v2/build/pico-sdk/src/rp2040/boot_stage2 && /usr/bin/cmake -E touch_nocreate /home/menadue/pico/framework/framework_16/macropad/v2/CMakeLists.txt

picotoolForceReconfigure: pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure
picotoolForceReconfigure: pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure.dir/build.make
.PHONY : picotoolForceReconfigure

# Rule to build all files generated by this target.
pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure.dir/build: picotoolForceReconfigure
.PHONY : pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure.dir/build

pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure.dir/clean:
	cd /home/menadue/pico/framework/framework_16/macropad/v2/build/pico-sdk/src/rp2040/boot_stage2 && $(CMAKE_COMMAND) -P CMakeFiles/picotoolForceReconfigure.dir/cmake_clean.cmake
.PHONY : pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure.dir/clean

pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure.dir/depend:
	cd /home/menadue/pico/framework/framework_16/macropad/v2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/menadue/pico/framework/framework_16/macropad/v2 /home/menadue/pico/pico-sdk/src/rp2040/boot_stage2 /home/menadue/pico/framework/framework_16/macropad/v2/build /home/menadue/pico/framework/framework_16/macropad/v2/build/pico-sdk/src/rp2040/boot_stage2 /home/menadue/pico/framework/framework_16/macropad/v2/build/pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pico-sdk/src/rp2040/boot_stage2/CMakeFiles/picotoolForceReconfigure.dir/depend

