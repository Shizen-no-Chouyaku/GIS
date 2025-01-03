# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/linux/LeapCore/GIS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/linux/LeapCore/GIS/build

# Include any dependencies generated for this target.
include CMakeFiles/CustomGIS.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CustomGIS.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CustomGIS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CustomGIS.dir/flags.make

CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.o: /home/linux/LeapCore/GIS/src/Networking/TileFetcher.cpp
CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.o -MF CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.o -c /home/linux/LeapCore/GIS/src/Networking/TileFetcher.cpp

CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/Networking/TileFetcher.cpp > CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.i

CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/Networking/TileFetcher.cpp -o CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.s

CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o: /home/linux/LeapCore/GIS/src/Rendering/TileRenderer.cpp
CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o -MF CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o -c /home/linux/LeapCore/GIS/src/Rendering/TileRenderer.cpp

CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/Rendering/TileRenderer.cpp > CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.i

CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/Rendering/TileRenderer.cpp -o CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.o: /home/linux/LeapCore/GIS/src/UI/Button.cpp
CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.o -c /home/linux/LeapCore/GIS/src/UI/Button.cpp

CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/Button.cpp > CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/Button.cpp -o CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o: /home/linux/LeapCore/GIS/src/UI/InputHandler.cpp
CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o -c /home/linux/LeapCore/GIS/src/UI/InputHandler.cpp

CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/InputHandler.cpp > CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/InputHandler.cpp -o CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.o: /home/linux/LeapCore/GIS/src/UI/LayerWindow.cpp
CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.o -c /home/linux/LeapCore/GIS/src/UI/LayerWindow.cpp

CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/LayerWindow.cpp > CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/LayerWindow.cpp -o CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.o: /home/linux/LeapCore/GIS/src/UI/MapWindow.cpp
CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.o -c /home/linux/LeapCore/GIS/src/UI/MapWindow.cpp

CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/MapWindow.cpp > CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/MapWindow.cpp -o CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.o: /home/linux/LeapCore/GIS/src/UI/Toolbar.cpp
CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.o -c /home/linux/LeapCore/GIS/src/UI/Toolbar.cpp

CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/Toolbar.cpp > CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/Toolbar.cpp -o CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o: /home/linux/LeapCore/GIS/src/UI/UIManager.cpp
CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o -c /home/linux/LeapCore/GIS/src/UI/UIManager.cpp

CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/UIManager.cpp > CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/UIManager.cpp -o CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.s

CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.o: /home/linux/LeapCore/GIS/src/Utils/MainLoop.cpp
CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.o -MF CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.o -c /home/linux/LeapCore/GIS/src/Utils/MainLoop.cpp

CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/Utils/MainLoop.cpp > CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.i

CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/Utils/MainLoop.cpp -o CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.s

CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o: /home/linux/LeapCore/GIS/src/Utils/SDLUtils.cpp
CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o -MF CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o -c /home/linux/LeapCore/GIS/src/Utils/SDLUtils.cpp

CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/Utils/SDLUtils.cpp > CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.i

CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/Utils/SDLUtils.cpp -o CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.s

CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o: /home/linux/LeapCore/GIS/src/Utils/Utils.cpp
CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o -MF CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o -c /home/linux/LeapCore/GIS/src/Utils/Utils.cpp

CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/Utils/Utils.cpp > CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.i

CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/Utils/Utils.cpp -o CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.s

CMakeFiles/CustomGIS.dir/src/main.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/main.cpp.o: /home/linux/LeapCore/GIS/src/main.cpp
CMakeFiles/CustomGIS.dir/src/main.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/CustomGIS.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/main.cpp.o -MF CMakeFiles/CustomGIS.dir/src/main.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/main.cpp.o -c /home/linux/LeapCore/GIS/src/main.cpp

CMakeFiles/CustomGIS.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/main.cpp > CMakeFiles/CustomGIS.dir/src/main.cpp.i

CMakeFiles/CustomGIS.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/main.cpp -o CMakeFiles/CustomGIS.dir/src/main.cpp.s

# Object files for target CustomGIS
CustomGIS_OBJECTS = \
"CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/main.cpp.o"

# External object files for target CustomGIS
CustomGIS_EXTERNAL_OBJECTS =

CustomGIS: CMakeFiles/CustomGIS.dir/src/Networking/TileFetcher.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/Button.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/LayerWindow.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/MapWindow.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/Toolbar.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/Utils/MainLoop.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/main.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/build.make
CustomGIS: CMakeFiles/CustomGIS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable CustomGIS"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CustomGIS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CustomGIS.dir/build: CustomGIS
.PHONY : CMakeFiles/CustomGIS.dir/build

CMakeFiles/CustomGIS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CustomGIS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CustomGIS.dir/clean

CMakeFiles/CustomGIS.dir/depend:
	cd /home/linux/LeapCore/GIS/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linux/LeapCore/GIS /home/linux/LeapCore/GIS /home/linux/LeapCore/GIS/build /home/linux/LeapCore/GIS/build /home/linux/LeapCore/GIS/build/CMakeFiles/CustomGIS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CustomGIS.dir/depend

