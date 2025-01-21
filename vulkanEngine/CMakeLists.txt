cmake_minimum_required(VERSION 3.16)
project(VulkanApp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Add GLFW (from the local folder)
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)

include_directories(lib)

# Add your source files
file(GLOB SOURCES src/*.cpp)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Volk, Vulkan, GLFW libraries
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)

target_link_options(VulkanApp PRIVATE -Wl,--allow-multiple-definition)
