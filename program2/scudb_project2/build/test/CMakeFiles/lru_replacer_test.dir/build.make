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
CMAKE_SOURCE_DIR = /mnt/hgfs/os_share/database/program2/scudb_project2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/os_share/database/program2/scudb_project2/build

# Include any dependencies generated for this target.
include test/CMakeFiles/lru_replacer_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/lru_replacer_test.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/lru_replacer_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/lru_replacer_test.dir/flags.make

test/CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.o: test/CMakeFiles/lru_replacer_test.dir/flags.make
test/CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.o: ../test/buffer/lru_replacer_test.cpp
test/CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.o: test/CMakeFiles/lru_replacer_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/os_share/database/program2/scudb_project2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.o"
	cd /mnt/hgfs/os_share/database/program2/scudb_project2/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.o -MF CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.o.d -o CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.o -c /mnt/hgfs/os_share/database/program2/scudb_project2/test/buffer/lru_replacer_test.cpp

test/CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.i"
	cd /mnt/hgfs/os_share/database/program2/scudb_project2/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/os_share/database/program2/scudb_project2/test/buffer/lru_replacer_test.cpp > CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.i

test/CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.s"
	cd /mnt/hgfs/os_share/database/program2/scudb_project2/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/os_share/database/program2/scudb_project2/test/buffer/lru_replacer_test.cpp -o CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.s

# Object files for target lru_replacer_test
lru_replacer_test_OBJECTS = \
"CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.o"

# External object files for target lru_replacer_test
lru_replacer_test_EXTERNAL_OBJECTS =

test/lru_replacer_test: test/CMakeFiles/lru_replacer_test.dir/buffer/lru_replacer_test.cpp.o
test/lru_replacer_test: test/CMakeFiles/lru_replacer_test.dir/build.make
test/lru_replacer_test: lib/libvtable.so
test/lru_replacer_test: lib/libsqlite3.so
test/lru_replacer_test: lib/libgtest.so
test/lru_replacer_test: test/CMakeFiles/lru_replacer_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/hgfs/os_share/database/program2/scudb_project2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable lru_replacer_test"
	cd /mnt/hgfs/os_share/database/program2/scudb_project2/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lru_replacer_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/lru_replacer_test.dir/build: test/lru_replacer_test
.PHONY : test/CMakeFiles/lru_replacer_test.dir/build

test/CMakeFiles/lru_replacer_test.dir/clean:
	cd /mnt/hgfs/os_share/database/program2/scudb_project2/build/test && $(CMAKE_COMMAND) -P CMakeFiles/lru_replacer_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/lru_replacer_test.dir/clean

test/CMakeFiles/lru_replacer_test.dir/depend:
	cd /mnt/hgfs/os_share/database/program2/scudb_project2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/os_share/database/program2/scudb_project2 /mnt/hgfs/os_share/database/program2/scudb_project2/test /mnt/hgfs/os_share/database/program2/scudb_project2/build /mnt/hgfs/os_share/database/program2/scudb_project2/build/test /mnt/hgfs/os_share/database/program2/scudb_project2/build/test/CMakeFiles/lru_replacer_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/lru_replacer_test.dir/depend

