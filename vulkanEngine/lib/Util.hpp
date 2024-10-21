#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define OBJECT_COUNT 1000
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vulkan_mem_alloc.h"
#include <functional>
#include <vector>
#include <stdexcept>
#include <bitset>
#include <

#define SCREEN_WIDTH 1600.0f
#define SCREEN_HEIGHT 900.0f

#define STORAGE_MB size_t(1048576)

#define MAX_LIGHT_COUNT 100
#define MAX_MATERIAL_COUNT 100


#define QUEUE_NOT_FOUND 3131


struct InstanceVariables {

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VmaAllocator allocator;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue computeQueue;
	VkQueue transferQueue;
	VkQueue presentQueue;
	GLFWwindow* window;
	VkSwapchainKHR swapchain;
	std::vector<VkFramebuffer> swapchainFramebuffers;
	VkExtent2D swapchainExtent;
	VkSurfaceKHR surface;
	VkRenderPass renderpass;
	VkBool32 multiDrawSupport;
	uint32_t minImageCount;
	uint32_t WIDTH;
	uint32_t HEIGHT;


};
// Assume you have a Material struct to hold material properties
struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	float transparency;
	int textureID;  // To reference a texture if necessary
};


struct MaterialBufferObject {
	glm::vec4 ambient[100];
	glm::vec4 diffuse[100]; // 4.component of diffuse is transparency
	glm::vec4 specular[100]; // 4.component of specular is shininess
	glm::uvec4 textureID[100];  // To reference a texture if necessary
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec4 color;
	glm::vec2 texCoord;
	uint32_t ID;
	uint32_t materialID;

	bool operator==(const Vertex& other) const {
		return pos == other.pos &&
			normal == other.normal &&
			color == other.color &&
			texCoord == other.texCoord &&
			ID == other.ID &&
			materialID == other.materialID;
	}
};

// Custom hash function for Vertex
struct VertexHash {
	std::size_t operator()(const Vertex& vertex) const {
		std::size_t seed = 0;

		// Hash combine function
		auto hashCombine = [&seed](auto& val) {
			std::hash<std::decay_t<decltype(val)>> hasher;
			seed ^= hasher(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			};

		// Hash each member of the Vertex struct
		hashCombine(vertex.pos.x);
		hashCombine(vertex.pos.y);
		hashCombine(vertex.pos.z);

		hashCombine(vertex.normal.x);
		hashCombine(vertex.normal.y);
		hashCombine(vertex.normal.z);

		hashCombine(vertex.color.r);
		hashCombine(vertex.color.g);
		hashCombine(vertex.color.b);
		hashCombine(vertex.color.a);

		hashCombine(vertex.texCoord.x);
		hashCombine(vertex.texCoord.y);

		hashCombine(vertex.ID);
		hashCombine(vertex.materialID);

		return seed;
	}
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
	glm::vec4 cameraPos;
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