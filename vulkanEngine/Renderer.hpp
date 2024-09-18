#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <stdexcept>


#include "Util.hpp"
#include "Object.hpp"

struct UniformBufferObject {
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 model[1000];
};



struct QueueFamilyIndices {
	uint32_t graphicsFamily;
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Renderer {
public:
	Renderer();
	~Renderer();

	void drawFrame();
	void addObject(Object*);
	void updateUniformBuffer(uint32_t targetFrame);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t index);
	void createSyncObjects();

	VkInstance instance;
	VkDevice device;
	VkPhysicalDevice pDevice;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	GLFWwindow* window;
	VkSwapchainKHR swapChain;

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;

	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_8_BIT;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkExtent2D swapChainExtent;

	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<Vertex> vertices;

	std::vector<uint32_t> indices;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkRenderPass renderPass;

	uint32_t WIDTH = 800;
	uint32_t HEIGHT = 600;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

private:
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	std::vector<Object*> objects;

	const int MAX_FRAMES_IN_FLIGHT = 3;

	// CAMERA

	glm::vec3 position;
	float horizontalAngle;
	float verticalAngle;

	float lastX;
	float lastY;
	bool firstMouse;

	float speed;
	float mouseSpeed;
	float lastTime;

	uint32_t currentFrame;
};