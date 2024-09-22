#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Util.hpp"
#include "Object.hpp"
#include "Texture.hpp"


class RenderBatch {

public:
	RenderBatch();
	RenderBatch(std::string& name, InstanceVariables& vars, const char* vertexPath, const char* fragmentPath);
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
	void updateUniformBuffer(uint32_t targetFrame, glm::vec3& position, glm::vec3& direction, glm::vec3& up);
	void createGraphicsPipeline();


	std::string name;

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



	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;

	uint32_t WIDTH = 1600;
	uint32_t HEIGHT = 900;

	
	UniformBufferObject ubo;
	std::vector<VkImageView> textureViews;
	std::vector<Texture*> textures;

	const char* vertexPath;
	const char* fragmentPath;

protected:

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void resetBuffers();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	static std::vector<char> readFile(const std::string& filename);


	void updateTextureDescriptors();
	void deleteTexture(Texture* texture);

	std::vector<Object*> objects;

	const int MAX_FRAMES_IN_FLIGHT = 3;

};