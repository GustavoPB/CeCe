# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.4.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.4.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Gus/GitHub_/CeCe

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Gus/GitHub_/CeCe/build

# Include any dependencies generated for this target.
include cece/loader/CMakeFiles/loader.dir/depend.make

# Include the progress variables for this target.
include cece/loader/CMakeFiles/loader.dir/progress.make

# Include the compile flags for this target's objects.
include cece/loader/CMakeFiles/loader.dir/flags.make

cece/loader/CMakeFiles/loader.dir/Loader.cpp.o: cece/loader/CMakeFiles/loader.dir/flags.make
cece/loader/CMakeFiles/loader.dir/Loader.cpp.o: ../cece/loader/Loader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Gus/GitHub_/CeCe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object cece/loader/CMakeFiles/loader.dir/Loader.cpp.o"
	cd /Users/Gus/GitHub_/CeCe/build/cece/loader && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/loader.dir/Loader.cpp.o -c /Users/Gus/GitHub_/CeCe/cece/loader/Loader.cpp

cece/loader/CMakeFiles/loader.dir/Loader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/loader.dir/Loader.cpp.i"
	cd /Users/Gus/GitHub_/CeCe/build/cece/loader && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Gus/GitHub_/CeCe/cece/loader/Loader.cpp > CMakeFiles/loader.dir/Loader.cpp.i

cece/loader/CMakeFiles/loader.dir/Loader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/loader.dir/Loader.cpp.s"
	cd /Users/Gus/GitHub_/CeCe/build/cece/loader && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Gus/GitHub_/CeCe/cece/loader/Loader.cpp -o CMakeFiles/loader.dir/Loader.cpp.s

cece/loader/CMakeFiles/loader.dir/Loader.cpp.o.requires:

.PHONY : cece/loader/CMakeFiles/loader.dir/Loader.cpp.o.requires

cece/loader/CMakeFiles/loader.dir/Loader.cpp.o.provides: cece/loader/CMakeFiles/loader.dir/Loader.cpp.o.requires
	$(MAKE) -f cece/loader/CMakeFiles/loader.dir/build.make cece/loader/CMakeFiles/loader.dir/Loader.cpp.o.provides.build
.PHONY : cece/loader/CMakeFiles/loader.dir/Loader.cpp.o.provides

cece/loader/CMakeFiles/loader.dir/Loader.cpp.o.provides.build: cece/loader/CMakeFiles/loader.dir/Loader.cpp.o


cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o: cece/loader/CMakeFiles/loader.dir/flags.make
cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o: ../cece/loader/FactoryManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Gus/GitHub_/CeCe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o"
	cd /Users/Gus/GitHub_/CeCe/build/cece/loader && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/loader.dir/FactoryManager.cpp.o -c /Users/Gus/GitHub_/CeCe/cece/loader/FactoryManager.cpp

cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/loader.dir/FactoryManager.cpp.i"
	cd /Users/Gus/GitHub_/CeCe/build/cece/loader && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Gus/GitHub_/CeCe/cece/loader/FactoryManager.cpp > CMakeFiles/loader.dir/FactoryManager.cpp.i

cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/loader.dir/FactoryManager.cpp.s"
	cd /Users/Gus/GitHub_/CeCe/build/cece/loader && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Gus/GitHub_/CeCe/cece/loader/FactoryManager.cpp -o CMakeFiles/loader.dir/FactoryManager.cpp.s

cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o.requires:

.PHONY : cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o.requires

cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o.provides: cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o.requires
	$(MAKE) -f cece/loader/CMakeFiles/loader.dir/build.make cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o.provides.build
.PHONY : cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o.provides

cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o.provides.build: cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o


loader: cece/loader/CMakeFiles/loader.dir/Loader.cpp.o
loader: cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o
loader: cece/loader/CMakeFiles/loader.dir/build.make

.PHONY : loader

# Rule to build all files generated by this target.
cece/loader/CMakeFiles/loader.dir/build: loader

.PHONY : cece/loader/CMakeFiles/loader.dir/build

cece/loader/CMakeFiles/loader.dir/requires: cece/loader/CMakeFiles/loader.dir/Loader.cpp.o.requires
cece/loader/CMakeFiles/loader.dir/requires: cece/loader/CMakeFiles/loader.dir/FactoryManager.cpp.o.requires

.PHONY : cece/loader/CMakeFiles/loader.dir/requires

cece/loader/CMakeFiles/loader.dir/clean:
	cd /Users/Gus/GitHub_/CeCe/build/cece/loader && $(CMAKE_COMMAND) -P CMakeFiles/loader.dir/cmake_clean.cmake
.PHONY : cece/loader/CMakeFiles/loader.dir/clean

cece/loader/CMakeFiles/loader.dir/depend:
	cd /Users/Gus/GitHub_/CeCe/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Gus/GitHub_/CeCe /Users/Gus/GitHub_/CeCe/cece/loader /Users/Gus/GitHub_/CeCe/build /Users/Gus/GitHub_/CeCe/build/cece/loader /Users/Gus/GitHub_/CeCe/build/cece/loader/CMakeFiles/loader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : cece/loader/CMakeFiles/loader.dir/depend

