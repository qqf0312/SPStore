# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/ex_sharding/deps/src/secp256k1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/ex_sharding/deps/src/secp256k1-build

# Utility rule file for ecmult_static_context.

# Include the progress variables for this target.
include CMakeFiles/ecmult_static_context.dir/progress.make

CMakeFiles/ecmult_static_context:
	cd /home/ubuntu/ex_sharding/deps/src/secp256k1 && /home/ubuntu/ex_sharding/deps/src/secp256k1-build/gen_context

ecmult_static_context: CMakeFiles/ecmult_static_context
ecmult_static_context: CMakeFiles/ecmult_static_context.dir/build.make

.PHONY : ecmult_static_context

# Rule to build all files generated by this target.
CMakeFiles/ecmult_static_context.dir/build: ecmult_static_context

.PHONY : CMakeFiles/ecmult_static_context.dir/build

CMakeFiles/ecmult_static_context.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ecmult_static_context.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ecmult_static_context.dir/clean

CMakeFiles/ecmult_static_context.dir/depend:
	cd /home/ubuntu/ex_sharding/deps/src/secp256k1-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/ex_sharding/deps/src/secp256k1 /home/ubuntu/ex_sharding/deps/src/secp256k1 /home/ubuntu/ex_sharding/deps/src/secp256k1-build /home/ubuntu/ex_sharding/deps/src/secp256k1-build /home/ubuntu/ex_sharding/deps/src/secp256k1-build/CMakeFiles/ecmult_static_context.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ecmult_static_context.dir/depend

