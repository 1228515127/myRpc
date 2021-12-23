# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/lsd/myRpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lsd/myRpc/build

# Include any dependencies generated for this target.
include example/server/CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include example/server/CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include example/server/CMakeFiles/server.dir/flags.make

example/server/CMakeFiles/server.dir/src/MyMathService.pb.o: example/server/CMakeFiles/server.dir/flags.make
example/server/CMakeFiles/server.dir/src/MyMathService.pb.o: ../example/server/src/MyMathService.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lsd/myRpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/server/CMakeFiles/server.dir/src/MyMathService.pb.o"
	cd /home/lsd/myRpc/build/example/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/src/MyMathService.pb.o -c /home/lsd/myRpc/example/server/src/MyMathService.pb.cc

example/server/CMakeFiles/server.dir/src/MyMathService.pb.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/src/MyMathService.pb.i"
	cd /home/lsd/myRpc/build/example/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lsd/myRpc/example/server/src/MyMathService.pb.cc > CMakeFiles/server.dir/src/MyMathService.pb.i

example/server/CMakeFiles/server.dir/src/MyMathService.pb.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/src/MyMathService.pb.s"
	cd /home/lsd/myRpc/build/example/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lsd/myRpc/example/server/src/MyMathService.pb.cc -o CMakeFiles/server.dir/src/MyMathService.pb.s

example/server/CMakeFiles/server.dir/src/server.o: example/server/CMakeFiles/server.dir/flags.make
example/server/CMakeFiles/server.dir/src/server.o: ../example/server/src/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lsd/myRpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object example/server/CMakeFiles/server.dir/src/server.o"
	cd /home/lsd/myRpc/build/example/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/src/server.o -c /home/lsd/myRpc/example/server/src/server.cpp

example/server/CMakeFiles/server.dir/src/server.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/src/server.i"
	cd /home/lsd/myRpc/build/example/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lsd/myRpc/example/server/src/server.cpp > CMakeFiles/server.dir/src/server.i

example/server/CMakeFiles/server.dir/src/server.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/src/server.s"
	cd /home/lsd/myRpc/build/example/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lsd/myRpc/example/server/src/server.cpp -o CMakeFiles/server.dir/src/server.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/src/MyMathService.pb.o" \
"CMakeFiles/server.dir/src/server.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

../example/server/bin/server: example/server/CMakeFiles/server.dir/src/MyMathService.pb.o
../example/server/bin/server: example/server/CMakeFiles/server.dir/src/server.o
../example/server/bin/server: example/server/CMakeFiles/server.dir/build.make
../example/server/bin/server: ../MyRpc/lib/libmyrpc.a
../example/server/bin/server: example/server/CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lsd/myRpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../../example/server/bin/server"
	cd /home/lsd/myRpc/build/example/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/server/CMakeFiles/server.dir/build: ../example/server/bin/server

.PHONY : example/server/CMakeFiles/server.dir/build

example/server/CMakeFiles/server.dir/clean:
	cd /home/lsd/myRpc/build/example/server && $(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : example/server/CMakeFiles/server.dir/clean

example/server/CMakeFiles/server.dir/depend:
	cd /home/lsd/myRpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lsd/myRpc /home/lsd/myRpc/example/server /home/lsd/myRpc/build /home/lsd/myRpc/build/example/server /home/lsd/myRpc/build/example/server/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/server/CMakeFiles/server.dir/depend

