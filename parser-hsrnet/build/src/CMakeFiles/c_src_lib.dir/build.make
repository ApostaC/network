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
CMAKE_SOURCE_DIR = /home/aposta/projects/network/parser-hsrnet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aposta/projects/network/parser-hsrnet/build

# Include any dependencies generated for this target.
include src/CMakeFiles/c_src_lib.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/c_src_lib.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/c_src_lib.dir/flags.make

src/CMakeFiles/c_src_lib.dir/utils.c.o: src/CMakeFiles/c_src_lib.dir/flags.make
src/CMakeFiles/c_src_lib.dir/utils.c.o: ../src/utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aposta/projects/network/parser-hsrnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/c_src_lib.dir/utils.c.o"
	cd /home/aposta/projects/network/parser-hsrnet/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/c_src_lib.dir/utils.c.o   -c /home/aposta/projects/network/parser-hsrnet/src/utils.c

src/CMakeFiles/c_src_lib.dir/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/c_src_lib.dir/utils.c.i"
	cd /home/aposta/projects/network/parser-hsrnet/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/aposta/projects/network/parser-hsrnet/src/utils.c > CMakeFiles/c_src_lib.dir/utils.c.i

src/CMakeFiles/c_src_lib.dir/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/c_src_lib.dir/utils.c.s"
	cd /home/aposta/projects/network/parser-hsrnet/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/aposta/projects/network/parser-hsrnet/src/utils.c -o CMakeFiles/c_src_lib.dir/utils.c.s

src/CMakeFiles/c_src_lib.dir/utils.c.o.requires:

.PHONY : src/CMakeFiles/c_src_lib.dir/utils.c.o.requires

src/CMakeFiles/c_src_lib.dir/utils.c.o.provides: src/CMakeFiles/c_src_lib.dir/utils.c.o.requires
	$(MAKE) -f src/CMakeFiles/c_src_lib.dir/build.make src/CMakeFiles/c_src_lib.dir/utils.c.o.provides.build
.PHONY : src/CMakeFiles/c_src_lib.dir/utils.c.o.provides

src/CMakeFiles/c_src_lib.dir/utils.c.o.provides.build: src/CMakeFiles/c_src_lib.dir/utils.c.o


# Object files for target c_src_lib
c_src_lib_OBJECTS = \
"CMakeFiles/c_src_lib.dir/utils.c.o"

# External object files for target c_src_lib
c_src_lib_EXTERNAL_OBJECTS =

src/libc_src_lib.a: src/CMakeFiles/c_src_lib.dir/utils.c.o
src/libc_src_lib.a: src/CMakeFiles/c_src_lib.dir/build.make
src/libc_src_lib.a: src/CMakeFiles/c_src_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aposta/projects/network/parser-hsrnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libc_src_lib.a"
	cd /home/aposta/projects/network/parser-hsrnet/build/src && $(CMAKE_COMMAND) -P CMakeFiles/c_src_lib.dir/cmake_clean_target.cmake
	cd /home/aposta/projects/network/parser-hsrnet/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/c_src_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/c_src_lib.dir/build: src/libc_src_lib.a

.PHONY : src/CMakeFiles/c_src_lib.dir/build

src/CMakeFiles/c_src_lib.dir/requires: src/CMakeFiles/c_src_lib.dir/utils.c.o.requires

.PHONY : src/CMakeFiles/c_src_lib.dir/requires

src/CMakeFiles/c_src_lib.dir/clean:
	cd /home/aposta/projects/network/parser-hsrnet/build/src && $(CMAKE_COMMAND) -P CMakeFiles/c_src_lib.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/c_src_lib.dir/clean

src/CMakeFiles/c_src_lib.dir/depend:
	cd /home/aposta/projects/network/parser-hsrnet/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aposta/projects/network/parser-hsrnet /home/aposta/projects/network/parser-hsrnet/src /home/aposta/projects/network/parser-hsrnet/build /home/aposta/projects/network/parser-hsrnet/build/src /home/aposta/projects/network/parser-hsrnet/build/src/CMakeFiles/c_src_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/c_src_lib.dir/depend

