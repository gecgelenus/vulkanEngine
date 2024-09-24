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

#define SCREEN_WIDTH 1600.0f
#define SCREEN_HEIGHT 900.0f

#define STORAGE_MB size_t(1048576)

#define MAX_LIGHT_COUNT 100


struct InstanceVariables {

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	GLFWwindow* window;
	VkSwapchainKHR swapchain;
	std::vector<VkFramebuffer> swapchainFramebuffers;
	VkExtent2D swapchainExtent;
	VkSurfaceKHR surface;
	VkRenderPass renderpass;
	uint32_t WIDTH;
	uint32_t HEIGHT;


};



struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoord;
	glm::uint32 ID;
};

struct CharInfo {
	int id, x, y, width, height, xoffset, yoffset, xadvance;
};

struct Kerning {
	int first, second, amount;
};


struct Vertex2D {
	glm::vec2 pos;
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

struct LightBufferObject {

	glm::vec4 lightPosArr[MAX_LIGHT_COUNT];
	glm::vec4 lightColorArr[MAX_LIGHT_COUNT];
	glm::vec4 ambientLight;
	glm::uint32 lightCount;
};

struct UniformBufferObjectVP {
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