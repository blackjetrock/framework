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
CMAKE_SOURCE_DIR = /home/menadue/pico/framework/framework_16/macropad/v1/build/_deps/picotool-src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/menadue/pico/framework/framework_16/macropad/v1/build/_deps/picotool-build

# Utility rule file for lib.

# Include any custom commands dependencies for this target.
include lib/mbedtls/library/CMakeFiles/lib.dir/compiler_depend.make

lib/mbedtls/library/CMakeFiles/lib: lib/mbedtls/library/libmbedcrypto.a
lib/mbedtls/library/CMakeFiles/lib: lib/mbedtls/library/libmbedx509.a
lib/mbedtls/library/CMakeFiles/lib: lib/mbedtls/library/libmbedtls.a

lib: lib/mbedtls/library/CMakeFiles/lib
lib: lib/mbedtls/library/CMakeFiles/lib.dir/build.make
.PHONY : lib

# Rule to build all files generated by this target.
lib/mbedtls/library/CMakeFiles/lib.dir/build: lib
.PHONY : lib/mbedtls/library/CMakeFiles/lib.dir/build

lib/mbedtls/library/CMakeFiles/lib.dir/clean:
	cd /home/menadue/pico/framework/framework_16/macropad/v1/build/_deps/picotool-build/lib/mbedtls/library && $(CMAKE_COMMAND) -P CMakeFiles/lib.dir/cmake_clean.cmake
.PHONY : lib/mbedtls/library/CMakeFiles/lib.dir/clean

lib/mbedtls/library/CMakeFiles/lib.dir/depend:
	cd /home/menadue/pico/framework/framework_16/macropad/v1/build/_deps/picotool-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/menadue/pico/framework/framework_16/macropad/v1/build/_deps/picotool-src /home/menadue/pico/pico-sdk/lib/mbedtls/library /home/menadue/pico/framework/framework_16/macropad/v1/build/_deps/picotool-build /home/menadue/pico/framework/framework_16/macropad/v1/build/_deps/picotool-build/lib/mbedtls/library /home/menadue/pico/framework/framework_16/macropad/v1/build/_deps/picotool-build/lib/mbedtls/library/CMakeFiles/lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/mbedtls/library/CMakeFiles/lib.dir/depend

