# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/talha/Documents/vulkanEngine/vulkanEngine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/talha/Documents/vulkanEngine/vulkanEngine/build

# Include any dependencies generated for this target.
include CMakeFiles/VulkanApp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/VulkanApp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/VulkanApp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/VulkanApp.dir/flags.make

CMakeFiles/VulkanApp.dir/src/Font.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/Font.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/Font.cpp
CMakeFiles/VulkanApp.dir/src/Font.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/VulkanApp.dir/src/Font.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/Font.cpp.o -MF CMakeFiles/VulkanApp.dir/src/Font.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/Font.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/Font.cpp

CMakeFiles/VulkanApp.dir/src/Font.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/Font.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/Font.cpp > CMakeFiles/VulkanApp.dir/src/Font.cpp.i

CMakeFiles/VulkanApp.dir/src/Font.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/Font.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/Font.cpp -o CMakeFiles/VulkanApp.dir/src/Font.cpp.s

CMakeFiles/VulkanApp.dir/src/Light.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/Light.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/Light.cpp
CMakeFiles/VulkanApp.dir/src/Light.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/VulkanApp.dir/src/Light.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/Light.cpp.o -MF CMakeFiles/VulkanApp.dir/src/Light.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/Light.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/Light.cpp

CMakeFiles/VulkanApp.dir/src/Light.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/Light.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/Light.cpp > CMakeFiles/VulkanApp.dir/src/Light.cpp.i

CMakeFiles/VulkanApp.dir/src/Light.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/Light.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/Light.cpp -o CMakeFiles/VulkanApp.dir/src/Light.cpp.s

CMakeFiles/VulkanApp.dir/src/Object.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/Object.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/Object.cpp
CMakeFiles/VulkanApp.dir/src/Object.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/VulkanApp.dir/src/Object.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/Object.cpp.o -MF CMakeFiles/VulkanApp.dir/src/Object.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/Object.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/Object.cpp

CMakeFiles/VulkanApp.dir/src/Object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/Object.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/Object.cpp > CMakeFiles/VulkanApp.dir/src/Object.cpp.i

CMakeFiles/VulkanApp.dir/src/Object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/Object.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/Object.cpp -o CMakeFiles/VulkanApp.dir/src/Object.cpp.s

CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/ObjectReader.cpp
CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.o -MF CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/ObjectReader.cpp

CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/ObjectReader.cpp > CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.i

CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/ObjectReader.cpp -o CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.s

CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/ObjectText.cpp
CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.o -MF CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/ObjectText.cpp

CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/ObjectText.cpp > CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.i

CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/ObjectText.cpp -o CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.s

CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderBatch.cpp
CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.o -MF CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderBatch.cpp

CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderBatch.cpp > CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.i

CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderBatch.cpp -o CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.s

CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderBatchText.cpp
CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.o -MF CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderBatchText.cpp

CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderBatchText.cpp > CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.i

CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderBatchText.cpp -o CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.s

CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderQueue.cpp
CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.o -MF CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderQueue.cpp

CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderQueue.cpp > CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.i

CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/RenderQueue.cpp -o CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.s

CMakeFiles/VulkanApp.dir/src/Service.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/Service.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/Service.cpp
CMakeFiles/VulkanApp.dir/src/Service.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/VulkanApp.dir/src/Service.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/Service.cpp.o -MF CMakeFiles/VulkanApp.dir/src/Service.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/Service.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/Service.cpp

CMakeFiles/VulkanApp.dir/src/Service.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/Service.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/Service.cpp > CMakeFiles/VulkanApp.dir/src/Service.cpp.i

CMakeFiles/VulkanApp.dir/src/Service.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/Service.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/Service.cpp -o CMakeFiles/VulkanApp.dir/src/Service.cpp.s

CMakeFiles/VulkanApp.dir/src/Texture.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/Texture.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/Texture.cpp
CMakeFiles/VulkanApp.dir/src/Texture.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/VulkanApp.dir/src/Texture.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/Texture.cpp.o -MF CMakeFiles/VulkanApp.dir/src/Texture.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/Texture.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/Texture.cpp

CMakeFiles/VulkanApp.dir/src/Texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/Texture.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/Texture.cpp > CMakeFiles/VulkanApp.dir/src/Texture.cpp.i

CMakeFiles/VulkanApp.dir/src/Texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/Texture.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/Texture.cpp -o CMakeFiles/VulkanApp.dir/src/Texture.cpp.s

CMakeFiles/VulkanApp.dir/src/imgui.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/imgui.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui.cpp
CMakeFiles/VulkanApp.dir/src/imgui.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/VulkanApp.dir/src/imgui.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/imgui.cpp.o -MF CMakeFiles/VulkanApp.dir/src/imgui.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/imgui.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui.cpp

CMakeFiles/VulkanApp.dir/src/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/imgui.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui.cpp > CMakeFiles/VulkanApp.dir/src/imgui.cpp.i

CMakeFiles/VulkanApp.dir/src/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/imgui.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui.cpp -o CMakeFiles/VulkanApp.dir/src/imgui.cpp.s

CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_demo.cpp
CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.o -MF CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_demo.cpp

CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_demo.cpp > CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.i

CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_demo.cpp -o CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.s

CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_draw.cpp
CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.o -MF CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_draw.cpp

CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_draw.cpp > CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.i

CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_draw.cpp -o CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.s

CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_impl_glfw.cpp
CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.o -MF CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_impl_glfw.cpp

CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_impl_glfw.cpp > CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.i

CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_impl_glfw.cpp -o CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.s

CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_impl_vulkan.cpp
CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.o -MF CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_impl_vulkan.cpp

CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_impl_vulkan.cpp > CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.i

CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_impl_vulkan.cpp -o CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.s

CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_tables.cpp
CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.o -MF CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_tables.cpp

CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_tables.cpp > CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.i

CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_tables.cpp -o CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.s

CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_widgets.cpp
CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.o -MF CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_widgets.cpp

CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_widgets.cpp > CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.i

CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/imgui_widgets.cpp -o CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.s

CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.o: CMakeFiles/VulkanApp.dir/flags.make
CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.o: /home/talha/Documents/vulkanEngine/vulkanEngine/src/vulkanEngine.cpp
CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.o: CMakeFiles/VulkanApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.o -MF CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.o.d -o CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.o -c /home/talha/Documents/vulkanEngine/vulkanEngine/src/vulkanEngine.cpp

CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/talha/Documents/vulkanEngine/vulkanEngine/src/vulkanEngine.cpp > CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.i

CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/talha/Documents/vulkanEngine/vulkanEngine/src/vulkanEngine.cpp -o CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.s

# Object files for target VulkanApp
VulkanApp_OBJECTS = \
"CMakeFiles/VulkanApp.dir/src/Font.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/Light.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/Object.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/Service.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/Texture.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/imgui.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.o" \
"CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.o"

# External object files for target VulkanApp
VulkanApp_EXTERNAL_OBJECTS =

VulkanApp: CMakeFiles/VulkanApp.dir/src/Font.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/Light.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/Object.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/ObjectReader.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/ObjectText.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/RenderBatch.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/RenderBatchText.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/RenderQueue.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/Service.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/Texture.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/imgui.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/imgui_demo.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/imgui_draw.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/imgui_impl_glfw.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/imgui_impl_vulkan.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/imgui_tables.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/imgui_widgets.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/src/vulkanEngine.cpp.o
VulkanApp: CMakeFiles/VulkanApp.dir/build.make
VulkanApp: /usr/lib/x86_64-linux-gnu/libvulkan.so
VulkanApp: lib/glfw/src/libglfw3.a
VulkanApp: /usr/lib/x86_64-linux-gnu/librt.a
VulkanApp: /usr/lib/x86_64-linux-gnu/libm.so
VulkanApp: CMakeFiles/VulkanApp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Linking CXX executable VulkanApp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/VulkanApp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/VulkanApp.dir/build: VulkanApp
.PHONY : CMakeFiles/VulkanApp.dir/build

CMakeFiles/VulkanApp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/VulkanApp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/VulkanApp.dir/clean

CMakeFiles/VulkanApp.dir/depend:
	cd /home/talha/Documents/vulkanEngine/vulkanEngine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/talha/Documents/vulkanEngine/vulkanEngine /home/talha/Documents/vulkanEngine/vulkanEngine /home/talha/Documents/vulkanEngine/vulkanEngine/build /home/talha/Documents/vulkanEngine/vulkanEngine/build /home/talha/Documents/vulkanEngine/vulkanEngine/build/CMakeFiles/VulkanApp.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/VulkanApp.dir/depend

