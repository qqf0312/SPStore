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

# Utility rule file for mysqlclient.

# Include the progress variables for this target.
include CMakeFiles/mysqlclient.dir/progress.make

CMakeFiles/mysqlclient: CMakeFiles/mysqlclient-complete


CMakeFiles/mysqlclient-complete: deps/src/mysqlclient-stamp/mysqlclient-install
CMakeFiles/mysqlclient-complete: deps/src/mysqlclient-stamp/mysqlclient-mkdir
CMakeFiles/mysqlclient-complete: deps/src/mysqlclient-stamp/mysqlclient-download
CMakeFiles/mysqlclient-complete: deps/src/mysqlclient-stamp/mysqlclient-update
CMakeFiles/mysqlclient-complete: deps/src/mysqlclient-stamp/mysqlclient-patch
CMakeFiles/mysqlclient-complete: deps/src/mysqlclient-stamp/mysqlclient-configure
CMakeFiles/mysqlclient-complete: deps/src/mysqlclient-stamp/mysqlclient-build
CMakeFiles/mysqlclient-complete: deps/src/mysqlclient-stamp/mysqlclient-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'mysqlclient'"
	/usr/bin/cmake -E make_directory /home/ubuntu/ex_sharding/CMakeFiles
	/usr/bin/cmake -E touch /home/ubuntu/ex_sharding/CMakeFiles/mysqlclient-complete
	/usr/bin/cmake -E touch /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-done

deps/src/mysqlclient-stamp/mysqlclient-install: deps/src/mysqlclient-stamp/mysqlclient-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing install step for 'mysqlclient'"
	cd /home/ubuntu/ex_sharding/deps/src/mysqlclient && /usr/bin/cmake -Dmake=$(MAKE) -P /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-install-Debug.cmake
	cd /home/ubuntu/ex_sharding/deps/src/mysqlclient && /usr/bin/cmake -E touch /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-install

deps/src/mysqlclient-stamp/mysqlclient-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'mysqlclient'"
	/usr/bin/cmake -E make_directory /home/ubuntu/ex_sharding/deps/src/mysqlclient
	/usr/bin/cmake -E make_directory /home/ubuntu/ex_sharding/deps/src/mysqlclient
	/usr/bin/cmake -E make_directory /home/ubuntu/ex_sharding/deps
	/usr/bin/cmake -E make_directory /home/ubuntu/ex_sharding/deps/tmp
	/usr/bin/cmake -E make_directory /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp
	/usr/bin/cmake -E make_directory /home/ubuntu/ex_sharding/deps/src
	/usr/bin/cmake -E touch /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-mkdir

deps/src/mysqlclient-stamp/mysqlclient-download: deps/src/mysqlclient-stamp/mysqlclient-urlinfo.txt
deps/src/mysqlclient-stamp/mysqlclient-download: deps/src/mysqlclient-stamp/mysqlclient-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (verify and extract) for 'mysqlclient'"
	cd /home/ubuntu/ex_sharding/deps/src && /usr/bin/cmake -P /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/verify-mysqlclient.cmake
	cd /home/ubuntu/ex_sharding/deps/src && /usr/bin/cmake -P /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/extract-mysqlclient.cmake
	cd /home/ubuntu/ex_sharding/deps/src && /usr/bin/cmake -E touch /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-download

deps/src/mysqlclient-stamp/mysqlclient-update: deps/src/mysqlclient-stamp/mysqlclient-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No update step for 'mysqlclient'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-update

deps/src/mysqlclient-stamp/mysqlclient-patch: deps/src/mysqlclient-stamp/mysqlclient-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'mysqlclient'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-patch

deps/src/mysqlclient-stamp/mysqlclient-configure: deps/tmp/mysqlclient-cfgcmd.txt
deps/src/mysqlclient-stamp/mysqlclient-configure: deps/src/mysqlclient-stamp/mysqlclient-update
deps/src/mysqlclient-stamp/mysqlclient-configure: deps/src/mysqlclient-stamp/mysqlclient-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'mysqlclient'"
	cd /home/ubuntu/ex_sharding/deps/src/mysqlclient && /usr/bin/cmake -P /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-configure-Debug.cmake
	cd /home/ubuntu/ex_sharding/deps/src/mysqlclient && /usr/bin/cmake -E touch /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-configure

deps/src/mysqlclient-stamp/mysqlclient-build: deps/src/mysqlclient-stamp/mysqlclient-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ubuntu/ex_sharding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'mysqlclient'"
	cd /home/ubuntu/ex_sharding/deps/src/mysqlclient && /usr/bin/cmake -Dmake=$(MAKE) -P /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-build-Debug.cmake
	cd /home/ubuntu/ex_sharding/deps/src/mysqlclient && /usr/bin/cmake -E touch /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-build

mysqlclient: CMakeFiles/mysqlclient
mysqlclient: CMakeFiles/mysqlclient-complete
mysqlclient: deps/src/mysqlclient-stamp/mysqlclient-install
mysqlclient: deps/src/mysqlclient-stamp/mysqlclient-mkdir
mysqlclient: deps/src/mysqlclient-stamp/mysqlclient-download
mysqlclient: deps/src/mysqlclient-stamp/mysqlclient-update
mysqlclient: deps/src/mysqlclient-stamp/mysqlclient-patch
mysqlclient: deps/src/mysqlclient-stamp/mysqlclient-configure
mysqlclient: deps/src/mysqlclient-stamp/mysqlclient-build
mysqlclient: CMakeFiles/mysqlclient.dir/build.make

.PHONY : mysqlclient

# Rule to build all files generated by this target.
CMakeFiles/mysqlclient.dir/build: mysqlclient

.PHONY : CMakeFiles/mysqlclient.dir/build

CMakeFiles/mysqlclient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mysqlclient.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mysqlclient.dir/clean

CMakeFiles/mysqlclient.dir/depend:
	cd /home/ubuntu/ex_sharding && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/ex_sharding /home/ubuntu/ex_sharding /home/ubuntu/ex_sharding /home/ubuntu/ex_sharding /home/ubuntu/ex_sharding/CMakeFiles/mysqlclient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mysqlclient.dir/depend

