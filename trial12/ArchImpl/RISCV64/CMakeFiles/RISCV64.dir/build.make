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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anoushka/workspace/etiss

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anoushka/workspace/etiss/trial12

# Include any dependencies generated for this target.
include ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/depend.make

# Include the progress variables for this target.
include ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/progress.make

# Include the compile flags for this target's objects.
include ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/flags.make

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.o: ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/flags.make
ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.o: ../ArchImpl/RISCV64/RISCV64Arch.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anoushka/workspace/etiss/trial12/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.o"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.o -c /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64Arch.cpp

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.i"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64Arch.cpp > CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.i

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.s"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64Arch.cpp -o CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.s

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.o: ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/flags.make
ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.o: ../ArchImpl/RISCV64/RISCV64ArchLib.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anoushka/workspace/etiss/trial12/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.o"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.o -c /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64ArchLib.cpp

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.i"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64ArchLib.cpp > CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.i

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.s"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64ArchLib.cpp -o CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.s

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.o: ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/flags.make
ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.o: ../ArchImpl/RISCV64/RISCV64MMU.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anoushka/workspace/etiss/trial12/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.o"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.o -c /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64MMU.cpp

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.i"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64MMU.cpp > CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.i

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.s"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64MMU.cpp -o CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.s

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.o: ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/flags.make
ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.o: ../ArchImpl/RISCV64/RISCV64Timer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anoushka/workspace/etiss/trial12/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.o"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.o -c /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64Timer.cpp

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.i"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64Timer.cpp > CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.i

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.s"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anoushka/workspace/etiss/ArchImpl/RISCV64/RISCV64Timer.cpp -o CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.s

# Object files for target RISCV64
RISCV64_OBJECTS = \
"CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.o" \
"CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.o" \
"CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.o" \
"CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.o"

# External object files for target RISCV64
RISCV64_EXTERNAL_OBJECTS =

ArchImpl/RISCV64/libRISCV64.so: ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Arch.cpp.o
ArchImpl/RISCV64/libRISCV64.so: ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64ArchLib.cpp.o
ArchImpl/RISCV64/libRISCV64.so: ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64MMU.cpp.o
ArchImpl/RISCV64/libRISCV64.so: ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/RISCV64Timer.cpp.o
ArchImpl/RISCV64/libRISCV64.so: ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/build.make
ArchImpl/RISCV64/libRISCV64.so: libETISS.so
ArchImpl/RISCV64/libRISCV64.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
ArchImpl/RISCV64/libRISCV64.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
ArchImpl/RISCV64/libRISCV64.so: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
ArchImpl/RISCV64/libRISCV64.so: ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/anoushka/workspace/etiss/trial12/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library libRISCV64.so"
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RISCV64.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/build: ArchImpl/RISCV64/libRISCV64.so

.PHONY : ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/build

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/clean:
	cd /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 && $(CMAKE_COMMAND) -P CMakeFiles/RISCV64.dir/cmake_clean.cmake
.PHONY : ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/clean

ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/depend:
	cd /home/anoushka/workspace/etiss/trial12 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anoushka/workspace/etiss /home/anoushka/workspace/etiss/ArchImpl/RISCV64 /home/anoushka/workspace/etiss/trial12 /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64 /home/anoushka/workspace/etiss/trial12/ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ArchImpl/RISCV64/CMakeFiles/RISCV64.dir/depend
