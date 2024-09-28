#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Util.hpp"
#include "Object.hpp"
#include "Texture.hpp"
#include "Light.hpp"
#include <unordered_map>


class RenderBatch {

public:
	RenderBatch();
	RenderBatch(std::string& name, InstanceVariables& vars, const char* vertexPath, const char* fragmentPath);
	~RenderBatch();

	void addObject(Object*);
	void addLight(Light*);

	void deleteObject(std::string& name);
	void deleteLight(uint32_t ID);

	Object* getObject(std::string& name);
	Object* getObject(const char* name);

	void setObjectTexture(std::string& name, Texture* texture);
	void setAmbientLight(const glm::vec4& ambient);

	void readOBJ(std::string& path);
	void readOBJ(const char* path);

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
	void updateUniformBuffer(uint32_t targetFrame, glm::vec3& position, glm::vec3& direction, glm::vec3& up, float FOV, float nearPlane, float farPlane);
	void updateLightBuffer(uint32_t targetFrame);
	void createGraphicsPipeline();

	std::unordered_map<std::string, int> textureMap;

	std::string name;

	VkInstance instance;
	VkDevice device;
	VkPhysicalDevice pDevice;
	VmaAllocator allocator;

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


	bool renderFlag;

	std::vector<Vertex> vertices;

	std::vector<uint32_t> indices;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VmaAllocation> uniformBuffersAllocation;
	std::vector<void*> uniformBuffersMapped;

	std::vector<VkBuffer> lightBuffers;
	std::vector<VmaAllocation> lightBuffersAllocation;
	std::vector<void*> lightBuffersMapped;

	std::vector<VkBuffer> objectPropertyBuffers;
	std::vector<VmaAllocation> objectPropertyBuffersAllocation;
	std::vector<void*> objectPropertyBuffersMapped;

	std::vector<VkBuffer> materialBuffers;
	std::vector<VmaAllocation> materialBuffersAllocation;
	std::vector<void*> materialBuffersMapped;

	VkRenderPass renderPass;
	VkSampler textureSampler;



	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSetLayout descriptorSetLayoutLight;

	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkDescriptorSet> descriptorSetsLight;
	LightBufferObject lbo;

	uint32_t WIDTH = 1600;
	uint32_t HEIGHT = 900;

	
	UniformBufferObject ubo;
	std::vector<VkImageView> textureViews;
	std::vector<Texture*> textures;

	const char* vertexPath;
	const char* fragmentPath;
	void resetBuffers();
	std::vector<Object*> objects;
	std::vector<Light*> lights;
	MaterialBufferObject mbo;

protected:

	VmaAllocation vertexBufferAllocation;
	VmaAllocation indexBufferAllocation;



	VmaAllocationInfo createBuffer(VkDeviceSize size, int memoryType, VkBufferUsageFlags usage, VkBuffer& buffer, VmaAllocation& allocation);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	VkShaderModule createShaderModule(const std::vector<char>& code);
	static std::vector<char> readFile(const std::string& filename);


	void updateTextureDescriptors();
	void deleteTexture(Texture* texture);

	

	uint32_t materialCount;

	const int MAX_FRAMES_IN_FLIGHT = 3;

};