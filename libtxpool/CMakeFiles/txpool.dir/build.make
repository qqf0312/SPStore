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
include libtxpool/CMakeFiles/txpool.dir/depend.make

# Include the progress variables for this target.
include libtxpool/CMakeFiles/txpool.dir/progress.make

# Include the compile flags for this target's objects.
include libtxpool/CMakeFiles/txpool.dir/flags.make

libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o: libtxpool/CMakeFiles/txpool.dir/flags.make
libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o: libtxpool/CommonTransactionNonceCheck.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o"
	cd /home/ubuntu/ex_sharding/libtxpool && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o -c /home/ubuntu/ex_sharding/libtxpool/CommonTransactionNonceCheck.cpp

libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.i"
	cd /home/ubuntu/ex_sharding/libtxpool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libtxpool/CommonTransactionNonceCheck.cpp > CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.i

libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.s"
	cd /home/ubuntu/ex_sharding/libtxpool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libtxpool/CommonTransactionNonceCheck.cpp -o CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.s

libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o.requires:

.PHONY : libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o.requires

libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o.provides: libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o.requires
	$(MAKE) -f libtxpool/CMakeFiles/txpool.dir/build.make libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o.provides.build
.PHONY : libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o.provides

libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o.provides.build: libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o


libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o: libtxpool/CMakeFiles/txpool.dir/flags.make
libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o: libtxpool/TransactionNonceCheck.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o"
	cd /home/ubuntu/ex_sharding/libtxpool && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o -c /home/ubuntu/ex_sharding/libtxpool/TransactionNonceCheck.cpp

libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.i"
	cd /home/ubuntu/ex_sharding/libtxpool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libtxpool/TransactionNonceCheck.cpp > CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.i

libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.s"
	cd /home/ubuntu/ex_sharding/libtxpool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libtxpool/TransactionNonceCheck.cpp -o CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.s

libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o.requires:

.PHONY : libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o.requires

libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o.provides: libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o.requires
	$(MAKE) -f libtxpool/CMakeFiles/txpool.dir/build.make libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o.provides.build
.PHONY : libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o.provides

libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o.provides.build: libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o


libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o: libtxpool/CMakeFiles/txpool.dir/flags.make
libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o: libtxpool/TxPool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o"
	cd /home/ubuntu/ex_sharding/libtxpool && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/txpool.dir/TxPool.cpp.o -c /home/ubuntu/ex_sharding/libtxpool/TxPool.cpp

libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/txpool.dir/TxPool.cpp.i"
	cd /home/ubuntu/ex_sharding/libtxpool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/ex_sharding/libtxpool/TxPool.cpp > CMakeFiles/txpool.dir/TxPool.cpp.i

libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/txpool.dir/TxPool.cpp.s"
	cd /home/ubuntu/ex_sharding/libtxpool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/ex_sharding/libtxpool/TxPool.cpp -o CMakeFiles/txpool.dir/TxPool.cpp.s

libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o.requires:

.PHONY : libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o.requires

libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o.provides: libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o.requires
	$(MAKE) -f libtxpool/CMakeFiles/txpool.dir/build.make libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o.provides.build
.PHONY : libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o.provides

libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o.provides.build: libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o


# Object files for target txpool
txpool_OBJECTS = \
"CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o" \
"CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o" \
"CMakeFiles/txpool.dir/TxPool.cpp.o"

# External object files for target txpool
txpool_EXTERNAL_OBJECTS =

libtxpool/libtxpool.a: libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o
libtxpool/libtxpool.a: libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o
libtxpool/libtxpool.a: libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o
libtxpool/libtxpool.a: libtxpool/CMakeFiles/txpool.dir/build.make
libtxpool/libtxpool.a: libtxpool/CMakeFiles/txpool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libtxpool.a"
	cd /home/ubuntu/ex_sharding/libtxpool && $(CMAKE_COMMAND) -P CMakeFiles/txpool.dir/cmake_clean_target.cmake
	cd /home/ubuntu/ex_sharding/libtxpool && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/txpool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libtxpool/CMakeFiles/txpool.dir/build: libtxpool/libtxpool.a

.PHONY : libtxpool/CMakeFiles/txpool.dir/build

libtxpool/CMakeFiles/txpool.dir/requires: libtxpool/CMakeFiles/txpool.dir/CommonTransactionNonceCheck.cpp.o.requires
libtxpool/CMakeFiles/txpool.dir/requires: libtxpool/CMakeFiles/txpool.dir/TransactionNonceCheck.cpp.o.requires
libtxpool/CMakeFiles/txpool.dir/requires: libtxpool/CMakeFiles/txpool.dir/TxPool.cpp.o.requires

.PHONY : libtxpool/CMakeFiles/txpool.dir/requires

libtxpool/CMakeFiles/txpool.dir/clean:
	cd /home/ubuntu/ex_sharding/libtxpool && $(CMAKE_COMMAND) -P CMakeFiles/txpool.dir/cmake_clean.cmake
.PHONY : libtxpool/CMakeFiles/txpool.dir/clean

libtxpool/CMakeFiles/txpool.dir/depend:
	cd /home/ubuntu/ex_sharding && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/ex_sharding /home/ubuntu/ex_sharding/libtxpool /home/ubuntu/ex_sharding /home/ubuntu/ex_sharding/libtxpool /home/ubuntu/ex_sharding/libtxpool/CMakeFiles/txpool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libtxpool/CMakeFiles/txpool.dir/depend

