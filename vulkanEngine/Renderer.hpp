#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>






#include <vector>
#include <stdexcept>


#include "Util.hpp"
#include "Object.hpp"

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

class Renderer {
public:
	Renderer();
	~Renderer();

	
	void drawFrame();
	void addObject(Object*);
	void deleteObject(std::string& name);

	Object* getObject(std::string& name);


	void readOBJ(std::string& path);

	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();



	void createDescriptorPool();
	void createDescriptorSetLayout();
	void allocateDescriptorSets();
	void createTextureSampler();

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
	VkSurfaceKHR surface;

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;

	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_8_BIT;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkExtent2D swapChainExtent;


	std::vector<Vertex> vertices;

	std::vector<uint32_t> indices;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	std::vector<VkBuffer> objectPropertyBuffers;
	std::vector<VkDeviceMemory> objectPropertyBuffersMemory;
	std::vector<void*> objectPropertyBuffersMapped;


	VkRenderPass renderPass;
	VkSampler textureSampler;

	VkImageView textureImageView;
	VkImageView bunnyImageView;

	

	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;

	uint32_t WIDTH = 800;
	uint32_t HEIGHT = 600;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	UniformBufferObject ubo;

private:
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void resetBuffers();


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