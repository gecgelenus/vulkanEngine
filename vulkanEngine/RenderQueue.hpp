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
	inline static bool stateUI;

	static void processInputs(GLFWwindow* window, int key, int scancode, int action, int mods);
	void checkCollisions();
	

	void createCommandPool();
	void createCommandBuffers();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t index);
	void createSyncObjects();
	void initImGui();

	InstanceVariables instance;

private:
	void ImGuiWindow();

	void showLightsBasic();
	void showModelsBasic();



	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	std::vector<RenderBatch*> batchList;
	std::vector<RenderBatchText*> batchListText;

	uint32_t currentFrame;
	const int MAX_FRAMES_IN_FLIGHT = 3;

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

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
	
	// --------- model list --------------
	std::vector<std::vector<float>> xValuesModel;
	std::vector<std::vector<float>> yValuesModel;
	std::vector<std::vector<float>> zValuesModel;

	std::vector<std::vector<float>> scaleValuesModel;
	std::vector<std::vector<float>> xRotationValuesModel;
	std::vector<std::vector<float>> yRotationValuesModel;
	std::vector<std::vector<float>> zRotationValuesModel;


	// --------- light list --------------

	std::vector<std::vector<float>> sliderValuesLight;
	std::vector<std::vector<float>> sliderValuesLightColorRed;
	std::vector<std::vector<float>> sliderValuesLightColorGreen;
	std::vector<std::vector<float>> sliderValuesLightColorBlue;

	std::vector<std::vector<float>> xValuesLight;
	std::vector<std::vector<float>> yValuesLight;
	std::vector<std::vector<float>> zValuesLight;



};