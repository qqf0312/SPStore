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
CMAKE_SOURCE_DIR = /home/ubuntu/ex_sharding

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/ex_sharding

# Include any dependencies generated for this target.
include libbloomfilter/CMakeFiles/libbf_static.dir/depend.make

# Include the progress variables for this target.
include libbloomfilter/CMakeFiles/libbf_static.dir/progress.make

# Include the compile flags for this target's objects.
include libbloomfilter/CMakeFiles/libbf_static.dir/flags.make

libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o: libbloomfilter/CMakeFiles/libbf_static.dir/flags.make
libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o: libbloomfilter/src/bitvector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libbf_static.dir/src/bitvector.cpp.o -c /home/ubuntu/ex_sharding/libbloomfilter/src/bitvector.cpp

libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libbf_static.dir/src/bitvector.cpp.i"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libbloomfilter/src/bitvector.cpp > CMakeFiles/libbf_static.dir/src/bitvector.cpp.i

libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libbf_static.dir/src/bitvector.cpp.s"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libbloomfilter/src/bitvector.cpp -o CMakeFiles/libbf_static.dir/src/bitvector.cpp.s

libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o.requires:

.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o.requires

libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o.provides: libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o.requires
	$(MAKE) -f libbloomfilter/CMakeFiles/libbf_static.dir/build.make libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o.provides.build
.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o.provides

libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o.provides.build: libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o


libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o: libbloomfilter/CMakeFiles/libbf_static.dir/flags.make
libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o: libbloomfilter/src/counter_vector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o -c /home/ubuntu/ex_sharding/libbloomfilter/src/counter_vector.cpp

libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libbf_static.dir/src/counter_vector.cpp.i"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libbloomfilter/src/counter_vector.cpp > CMakeFiles/libbf_static.dir/src/counter_vector.cpp.i

libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libbf_static.dir/src/counter_vector.cpp.s"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libbloomfilter/src/counter_vector.cpp -o CMakeFiles/libbf_static.dir/src/counter_vector.cpp.s

libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o.requires:

.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o.requires

libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o.provides: libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o.requires
	$(MAKE) -f libbloomfilter/CMakeFiles/libbf_static.dir/build.make libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o.provides.build
.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o.provides

libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o.provides.build: libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o


libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o: libbloomfilter/CMakeFiles/libbf_static.dir/flags.make
libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o: libbloomfilter/src/hash.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libbf_static.dir/src/hash.cpp.o -c /home/ubuntu/ex_sharding/libbloomfilter/src/hash.cpp

libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libbf_static.dir/src/hash.cpp.i"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libbloomfilter/src/hash.cpp > CMakeFiles/libbf_static.dir/src/hash.cpp.i

libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libbf_static.dir/src/hash.cpp.s"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libbloomfilter/src/hash.cpp -o CMakeFiles/libbf_static.dir/src/hash.cpp.s

libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o.requires:

.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o.requires

libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o.provides: libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o.requires
	$(MAKE) -f libbloomfilter/CMakeFiles/libbf_static.dir/build.make libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o.provides.build
.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o.provides

libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o.provides.build: libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o


libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o: libbloomfilter/CMakeFiles/libbf_static.dir/flags.make
libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o: libbloomfilter/src/bloom_filter/a2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o -c /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/a2.cpp

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.i"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/a2.cpp > CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.i

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.s"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/a2.cpp -o CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.s

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o.requires:

.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o.requires

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o.provides: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o.requires
	$(MAKE) -f libbloomfilter/CMakeFiles/libbf_static.dir/build.make libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o.provides.build
.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o.provides

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o.provides.build: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o


libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o: libbloomfilter/CMakeFiles/libbf_static.dir/flags.make
libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o: libbloomfilter/src/bloom_filter/basic.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o -c /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/basic.cpp

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.i"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/basic.cpp > CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.i

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.s"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/basic.cpp -o CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.s

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o.requires:

.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o.requires

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o.provides: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o.requires
	$(MAKE) -f libbloomfilter/CMakeFiles/libbf_static.dir/build.make libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o.provides.build
.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o.provides

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o.provides.build: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o


libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o: libbloomfilter/CMakeFiles/libbf_static.dir/flags.make
libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o: libbloomfilter/src/bloom_filter/bitwise.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o -c /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/bitwise.cpp

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.i"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/bitwise.cpp > CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.i

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.s"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/bitwise.cpp -o CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.s

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o.requires:

.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o.requires

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o.provides: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o.requires
	$(MAKE) -f libbloomfilter/CMakeFiles/libbf_static.dir/build.make libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o.provides.build
.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o.provides

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o.provides.build: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o


libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o: libbloomfilter/CMakeFiles/libbf_static.dir/flags.make
libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o: libbloomfilter/src/bloom_filter/counting.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o -c /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/counting.cpp

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.i"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/counting.cpp > CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.i

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.s"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/counting.cpp -o CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.s

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o.requires:

.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o.requires

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o.provides: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o.requires
	$(MAKE) -f libbloomfilter/CMakeFiles/libbf_static.dir/build.make libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o.provides.build
.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o.provides

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o.provides.build: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o


libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o: libbloomfilter/CMakeFiles/libbf_static.dir/flags.make
libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o: libbloomfilter/src/bloom_filter/stable.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o -c /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/stable.cpp

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.i"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/stable.cpp > CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.i

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.s"
	cd /home/ubuntu/ex_sharding/libbloomfilter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libbloomfilter/src/bloom_filter/stable.cpp -o CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.s

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o.requires:

.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o.requires

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o.provides: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o.requires
	$(MAKE) -f libbloomfilter/CMakeFiles/libbf_static.dir/build.make libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o.provides.build
.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o.provides

libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o.provides.build: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o


# Object files for target libbf_static
libbf_static_OBJECTS = \
"CMakeFiles/libbf_static.dir/src/bitvector.cpp.o" \
"CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o" \
"CMakeFiles/libbf_static.dir/src/hash.cpp.o" \
"CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o" \
"CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o" \
"CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o" \
"CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o" \
"CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o"

# External object files for target libbf_static
libbf_static_EXTERNAL_OBJECTS =

libbloomfilter/libbf.a: libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o
libbloomfilter/libbf.a: libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o
libbloomfilter/libbf.a: libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o
libbloomfilter/libbf.a: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o
libbloomfilter/libbf.a: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o
libbloomfilter/libbf.a: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o
libbloomfilter/libbf.a: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o
libbloomfilter/libbf.a: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o
libbloomfilter/libbf.a: libbloomfilter/CMakeFiles/libbf_static.dir/build.make
libbloomfilter/libbf.a: libbloomfilter/CMakeFiles/libbf_static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX static library libbf.a"
	cd /home/ubuntu/ex_sharding/libbloomfilter && $(CMAKE_COMMAND) -P CMakeFiles/libbf_static.dir/cmake_clean_target.cmake
	cd /home/ubuntu/ex_sharding/libbloomfilter && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libbf_static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libbloomfilter/CMakeFiles/libbf_static.dir/build: libbloomfilter/libbf.a

.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/build

libbloomfilter/CMakeFiles/libbf_static.dir/requires: libbloomfilter/CMakeFiles/libbf_static.dir/src/bitvector.cpp.o.requires
libbloomfilter/CMakeFiles/libbf_static.dir/requires: libbloomfilter/CMakeFiles/libbf_static.dir/src/counter_vector.cpp.o.requires
libbloomfilter/CMakeFiles/libbf_static.dir/requires: libbloomfilter/CMakeFiles/libbf_static.dir/src/hash.cpp.o.requires
libbloomfilter/CMakeFiles/libbf_static.dir/requires: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/a2.cpp.o.requires
libbloomfilter/CMakeFiles/libbf_static.dir/requires: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/basic.cpp.o.requires
libbloomfilter/CMakeFiles/libbf_static.dir/requires: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/bitwise.cpp.o.requires
libbloomfilter/CMakeFiles/libbf_static.dir/requires: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/counting.cpp.o.requires
libbloomfilter/CMakeFiles/libbf_static.dir/requires: libbloomfilter/CMakeFiles/libbf_static.dir/src/bloom_filter/stable.cpp.o.requires

.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/requires

libbloomfilter/CMakeFiles/libbf_static.dir/clean:
	cd /home/ubuntu/ex_sharding/libbloomfilter && $(CMAKE_COMMAND) -P CMakeFiles/libbf_static.dir/cmake_clean.cmake
.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/clean

libbloomfilter/CMakeFiles/libbf_static.dir/depend:
	cd /home/ubuntu/ex_sharding && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/ex_sharding /home/ubuntu/ex_sharding/libbloomfilter /home/ubuntu/ex_sharding /home/ubuntu/ex_sharding/libbloomfilter /home/ubuntu/ex_sharding/libbloomfilter/CMakeFiles/libbf_static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libbloomfilter/CMakeFiles/libbf_static.dir/depend
