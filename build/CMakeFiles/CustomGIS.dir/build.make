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

CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.o: /home/linux/LeapCore/GIS/src/Config/ConfigManager.cpp
CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.o -MF CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.o -c /home/linux/LeapCore/GIS/src/Config/ConfigManager.cpp

CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/Config/ConfigManager.cpp > CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.i

CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/Config/ConfigManager.cpp -o CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.s

CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.o: /home/linux/LeapCore/GIS/src/Config/FontManager.cpp
CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.o -MF CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.o -c /home/linux/LeapCore/GIS/src/Config/FontManager.cpp

CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/Config/FontManager.cpp > CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.i

CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/Config/FontManager.cpp -o CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.s

CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.o: /home/linux/LeapCore/GIS/src/Networking/Tiles/TileFetcher.cpp
CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.o -MF CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.o -c /home/linux/LeapCore/GIS/src/Networking/Tiles/TileFetcher.cpp

CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/Networking/Tiles/TileFetcher.cpp > CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.i

CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/Networking/Tiles/TileFetcher.cpp -o CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.s

CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.o: /home/linux/LeapCore/GIS/src/Rendering/MainLoop.cpp
CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.o -MF CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.o -c /home/linux/LeapCore/GIS/src/Rendering/MainLoop.cpp

CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/Rendering/MainLoop.cpp > CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.i

CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/Rendering/MainLoop.cpp -o CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.s

CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o: /home/linux/LeapCore/GIS/src/Rendering/TileRenderer.cpp
CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o -MF CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o -c /home/linux/LeapCore/GIS/src/Rendering/TileRenderer.cpp

CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/Rendering/TileRenderer.cpp > CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.i

CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/Rendering/TileRenderer.cpp -o CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.o: /home/linux/LeapCore/GIS/src/UI/Components/Button.cpp
CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.o -c /home/linux/LeapCore/GIS/src/UI/Components/Button.cpp

CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/Components/Button.cpp > CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/Components/Button.cpp -o CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.o: /home/linux/LeapCore/GIS/src/UI/Components/Dropdown.cpp
CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.o -c /home/linux/LeapCore/GIS/src/UI/Components/Dropdown.cpp

CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/Components/Dropdown.cpp > CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/Components/Dropdown.cpp -o CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o: /home/linux/LeapCore/GIS/src/UI/InputHandler.cpp
CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o -c /home/linux/LeapCore/GIS/src/UI/InputHandler.cpp

CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/InputHandler.cpp > CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/InputHandler.cpp -o CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o: /home/linux/LeapCore/GIS/src/UI/UIManager.cpp
CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o -c /home/linux/LeapCore/GIS/src/UI/UIManager.cpp

CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/UIManager.cpp > CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/UIManager.cpp -o CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.o: /home/linux/LeapCore/GIS/src/UI/Windows/LayerWindow.cpp
CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.o -c /home/linux/LeapCore/GIS/src/UI/Windows/LayerWindow.cpp

CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/Windows/LayerWindow.cpp > CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/Windows/LayerWindow.cpp -o CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.o: /home/linux/LeapCore/GIS/src/UI/Windows/MapWindow.cpp
CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.o -c /home/linux/LeapCore/GIS/src/UI/Windows/MapWindow.cpp

CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/Windows/MapWindow.cpp > CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/Windows/MapWindow.cpp -o CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.o: /home/linux/LeapCore/GIS/src/UI/Windows/Settings/GeneralTab.cpp
CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.o -c /home/linux/LeapCore/GIS/src/UI/Windows/Settings/GeneralTab.cpp

CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/Windows/Settings/GeneralTab.cpp > CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/Windows/Settings/GeneralTab.cpp -o CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.o: /home/linux/LeapCore/GIS/src/UI/Windows/Settings/LayersTab.cpp
CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.o -c /home/linux/LeapCore/GIS/src/UI/Windows/Settings/LayersTab.cpp

CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/Windows/Settings/LayersTab.cpp > CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/Windows/Settings/LayersTab.cpp -o CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.o: /home/linux/LeapCore/GIS/src/UI/Windows/Settings/SettingsWindow.cpp
CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.o -c /home/linux/LeapCore/GIS/src/UI/Windows/Settings/SettingsWindow.cpp

CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/Windows/Settings/SettingsWindow.cpp > CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/Windows/Settings/SettingsWindow.cpp -o CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.s

CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.o: /home/linux/LeapCore/GIS/src/UI/Windows/Toolbar.cpp
CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.o -MF CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.o -c /home/linux/LeapCore/GIS/src/UI/Windows/Toolbar.cpp

CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/UI/Windows/Toolbar.cpp > CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.i

CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/UI/Windows/Toolbar.cpp -o CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.s

CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o: CMakeFiles/CustomGIS.dir/flags.make
CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o: /home/linux/LeapCore/GIS/src/Utils/SDLUtils.cpp
CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o: CMakeFiles/CustomGIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o"
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
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o"
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
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/CustomGIS.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CustomGIS.dir/src/main.cpp.o -MF CMakeFiles/CustomGIS.dir/src/main.cpp.o.d -o CMakeFiles/CustomGIS.dir/src/main.cpp.o -c /home/linux/LeapCore/GIS/src/main.cpp

CMakeFiles/CustomGIS.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CustomGIS.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/LeapCore/GIS/src/main.cpp > CMakeFiles/CustomGIS.dir/src/main.cpp.i

CMakeFiles/CustomGIS.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CustomGIS.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/LeapCore/GIS/src/main.cpp -o CMakeFiles/CustomGIS.dir/src/main.cpp.s

# Object files for target CustomGIS
CustomGIS_OBJECTS = \
"CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o" \
"CMakeFiles/CustomGIS.dir/src/main.cpp.o"

# External object files for target CustomGIS
CustomGIS_EXTERNAL_OBJECTS =

CustomGIS: CMakeFiles/CustomGIS.dir/src/Config/ConfigManager.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/Config/FontManager.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/Networking/Tiles/TileFetcher.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/Rendering/MainLoop.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/Rendering/TileRenderer.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/Components/Button.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/Components/Dropdown.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/InputHandler.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/UIManager.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/Windows/LayerWindow.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/Windows/MapWindow.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/GeneralTab.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/LayersTab.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/Windows/Settings/SettingsWindow.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/UI/Windows/Toolbar.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/Utils/SDLUtils.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/Utils/Utils.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/src/main.cpp.o
CustomGIS: CMakeFiles/CustomGIS.dir/build.make
CustomGIS: CMakeFiles/CustomGIS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/linux/LeapCore/GIS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Linking CXX executable CustomGIS"
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

