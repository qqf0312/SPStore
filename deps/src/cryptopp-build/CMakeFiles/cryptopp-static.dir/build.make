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
CMAKE_SOURCE_DIR = /home/ubuntu/ex_sharding/deps/src/cryptopp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/ex_sharding/deps/src/cryptopp-build

# Include any dependencies generated for this target.
include CMakeFiles/cryptopp-static.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cryptopp-static.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cryptopp-static.dir/flags.make

# Object files for target cryptopp-static
cryptopp__static_OBJECTS =

# External object files for target cryptopp-static
cryptopp__static_EXTERNAL_OBJECTS = \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/cryptlib.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/cpu.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/integer.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/algebra.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/algparam.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/asn.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/basecode.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/dll.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/dsa.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/ec2n.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/eccrypto.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/ecp.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/eprecomp.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/filters.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/fips140.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/gf2n.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/gfpcrypt.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/hex.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/hmac.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/hrtimer.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/iterhash.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/keccak.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/misc.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/modes.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/mqueue.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/nbtheory.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/oaep.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/osrng.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/polynomi.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/pubkey.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/queue.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/randpool.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/rdtables.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/rijndael.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/rng.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/sha.cpp.o" \
"/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-object.dir/strciphr.cpp.o"

libcryptopp.a: CMakeFiles/cryptopp-object.dir/cryptlib.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/cpu.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/integer.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/algebra.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/algparam.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/asn.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/basecode.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/dll.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/dsa.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/ec2n.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/eccrypto.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/ecp.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/eprecomp.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/filters.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/fips140.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/gf2n.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/gfpcrypt.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/hex.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/hmac.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/hrtimer.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/iterhash.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/keccak.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/misc.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/modes.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/mqueue.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/nbtheory.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/oaep.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/osrng.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/polynomi.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/pubkey.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/queue.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/randpool.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/rdtables.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/rijndael.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/rng.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/sha.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-object.dir/strciphr.cpp.o
libcryptopp.a: CMakeFiles/cryptopp-static.dir/build.make
libcryptopp.a: CMakeFiles/cryptopp-static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Linking CXX static library libcryptopp.a"
	$(CMAKE_COMMAND) -P CMakeFiles/cryptopp-static.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cryptopp-static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cryptopp-static.dir/build: libcryptopp.a

.PHONY : CMakeFiles/cryptopp-static.dir/build

CMakeFiles/cryptopp-static.dir/requires:

.PHONY : CMakeFiles/cryptopp-static.dir/requires

CMakeFiles/cryptopp-static.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cryptopp-static.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cryptopp-static.dir/clean

CMakeFiles/cryptopp-static.dir/depend:
	cd /home/ubuntu/ex_sharding/deps/src/cryptopp-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/ex_sharding/deps/src/cryptopp /home/ubuntu/ex_sharding/deps/src/cryptopp /home/ubuntu/ex_sharding/deps/src/cryptopp-build /home/ubuntu/ex_sharding/deps/src/cryptopp-build /home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/cryptopp-static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cryptopp-static.dir/depend

