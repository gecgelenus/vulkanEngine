#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Util.hpp"
#include "Object.hpp"
#include "Texture.hpp"


class RenderBatch {

public:
	RenderBatch();
	~RenderBatch();

	void addObject(Object*);
	void deleteObject(std::string& name);

	Object* getObject(std::string& name);
	void setObjectTexture(std::string& name, Texture* texture);

	void readOBJ(std::string& path);
	void createCommandPool();
	void createCommandBuffers();
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void addTexture(Texture* texture);

	void createDescriptorPool();
	void createDescriptorSetLayout();
	void allocateDescriptorSets();
	void createTextureSampler();
	void updateUniformBuffer(uint32_t targetFrame);



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



	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;

	uint32_t WIDTH = 1600;
	uint32_t HEIGHT = 900;

	
	UniformBufferObject ubo;


};