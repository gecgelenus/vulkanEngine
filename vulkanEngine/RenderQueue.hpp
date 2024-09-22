#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <vector>
#include <string>


#include "RenderBatch.hpp"
#include "RenderBatchText.hpp"


class RenderQueue {

public:
	RenderQueue(InstanceVariables& vars);
	~RenderQueue();

	void pushToQueue(RenderBatch* batch);
	void pushToQueue(RenderBatchText* batchText);

	void removeFromQueue(RenderBatch* batch);
	void removeFromQueue(RenderBatchText* batchText);

	void updateCommandBuffers();

	void drawFrame();
	void calculateViewVectors();

	void createCommandPool();
	void createCommandBuffers();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t index);
	void createSyncObjects();


	InstanceVariables instance;


private:

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	std::vector<RenderBatch*> batchList;
	std::vector<RenderBatchText*> batchListText;

	uint32_t currentFrame;
	const int MAX_FRAMES_IN_FLIGHT = 3;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;

	float horizontalAngle;
	float verticalAngle;

	float lastX;
	float lastY;
	bool firstMouse;

	float speed;
	float mouseSpeed;
	float lastTime;


};