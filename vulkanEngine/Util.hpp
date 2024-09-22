#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define OBJECT_COUNT 1000
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <stdexcept>

#define STORAGE_MB size_t(1048576)

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoord;
	glm::uint32 ID;
};


struct objectProperties {
	glm::uint objectID;
	glm::uint materialID;
	glm::uint textureID;
	glm::uint normalMapID;

};
struct UniformBufferObject {
	glm::mat4 model[1000];
	glm::mat4 view;
	glm::mat4 proj;

};



struct QueueFamilyIndices {
	uint32_t graphicsFamily;
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};