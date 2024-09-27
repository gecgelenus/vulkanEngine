#include "RenderQueue.hpp"
#include <iostream>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

static void check_vk_result(VkResult err)
{
	if (err == 0)
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}


RenderQueue::RenderQueue(InstanceVariables& vars)
{
	this->instance = vars;
	this->stateUI = false;


	position = glm::vec3(5, 5, 5);
	// horizontal angle : toward -Z
	horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	verticalAngle = 0.0f;

	speed = 3.0f; // 3 units / second
	mouseSpeed = 1.0f;

	createCommandPool();
	createCommandBuffers();
	createSyncObjects();
	glfwSetKeyCallback(instance.window, RenderQueue::processInputs);

}

RenderQueue::~RenderQueue()
{

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(instance.device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(instance.device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(instance.device, inFlightFences[i], nullptr);
	}


	vkDestroyCommandPool(instance.device, commandPool, nullptr);

}

void RenderQueue::pushToQueue(RenderBatch* batch)
{
	batchList.push_back(batch);
	std::cout << "Render queue -> batch added to queue: " << batch->name << std::endl;
}

void RenderQueue::pushToQueue(RenderBatchText* batchText)
{
	batchListText.push_back(batchText);
	std::cout << "Render queue -> batch added to (text) queue: " << batchText->name << std::endl;
	initImGui();

}

void RenderQueue::removeFromQueue(RenderBatch* batch)
{


	for (int i = 0; i < batchList.size(); i++) {
		if (batchList[i]->name == batch->name) {
			std::cout << "Render queue -> Deleting the batch: " << batch->name << std::endl;
			batchList.erase(batchList.begin() + i);
			return;
		}
	}

	std::cout << "Render queue -> Tried to delete a batch but it is not exists.: " << batch->name << std::endl;

}

void RenderQueue::removeFromQueue(RenderBatchText* batchText)
{
	for (int i = 0; i < batchList.size(); i++) {
		if (batchListText[i]->name == batchText->name) {
			std::cout << "Render queue -> Deleting the batch(text): " << batchText->name << std::endl;
			batchListText.erase(batchListText.begin() + i);
			return;
		}
	}

	std::cout << "Render queue -> Tried to delete a batch (text) but it is not exists.: " << batchText->name << std::endl;
}

void RenderQueue::updateCommandBuffers()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkResetCommandBuffer(commandBuffers[i], 0);
		recordCommandBuffer(commandBuffers[i], i);
	}
}

void RenderQueue::drawFrame()
{
	vkWaitForFences(instance.device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(instance.device, instance.swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	VkSwapchainKHR swapChains[] = { instance.swapchain };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;


	if (!stateUI) {
		calculateViewVectors();
	}
	for (RenderBatch* b : batchList) {
		b->updateUniformBuffer(imageIndex, position, direction, up);
		b->updateLightBuffer(imageIndex);
	}
	if (stateUI) {
		for (RenderBatchText* b : batchListText) {
			b->updateUniformBuffer(imageIndex, position, direction, up);
		}
	}
	recordCommandBuffer(commandBuffers[currentFrame], imageIndex);
	vkResetFences(instance.device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(instance.graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	vkQueuePresentKHR(instance.presentQueue, &presentInfo);
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;



}

void RenderQueue::calculateViewVectors()
{

	float currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	double xpos, ypos;

	glfwGetCursorPos(instance.window, &xpos, &ypos);

	glfwSetCursorPos(instance.window, instance.WIDTH / 2, instance.HEIGHT / 2);

	horizontalAngle += mouseSpeed * deltaTime * float(instance.WIDTH / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(instance.HEIGHT / 2 - ypos);

	const float maxVerticalAngle = glm::radians(89.0f);
	if (verticalAngle > maxVerticalAngle)
		verticalAngle = maxVerticalAngle;
	else if (verticalAngle < -maxVerticalAngle)
		verticalAngle = -maxVerticalAngle;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(instance.window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(instance.window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(instance.window, GLFW_KEY_D) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(instance.window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}
	// Move up
	if (glfwGetKey(instance.window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(instance.window, GLFW_KEY_SPACE) == GLFW_REPEAT) {
		position += up * deltaTime * speed;
	}
	// Move down
	if (glfwGetKey(instance.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(instance.window, GLFW_KEY_LEFT_CONTROL) == GLFW_REPEAT) {
		position -= up * deltaTime * speed;
	}
	// Terminate program
	if (glfwGetKey(instance.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		stateUI = true;

	}
}

void RenderQueue::processInputs(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (stateUI) {
			stateUI = false;
			glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		}
		else {
			stateUI = true;
			glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}

void RenderQueue::checkCollisions()
{


}

void RenderQueue::createCommandPool()
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = 0;

	if (vkCreateCommandPool(instance.device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void RenderQueue::createCommandBuffers()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(instance.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void RenderQueue::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t index)
{

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	VkClearValue depthColor{};
	depthColor.depthStencil = { 1.0f, 0 };

	VkClearValue clearColorArray[] = { clearColor, depthColor };

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; 
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}
	
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = instance.renderpass;
	renderPassInfo.framebuffer = instance.swapchainFramebuffers[index];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = instance.swapchainExtent;
	renderPassInfo.clearValueCount = 2;
	renderPassInfo.pClearValues = clearColorArray;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(instance.swapchainExtent.width);
	viewport.height = static_cast<float>(instance.swapchainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = instance.swapchainExtent;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


	for (int i = 0; i < batchList.size(); i++) {

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, batchList[i]->graphicsPipeline);


		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);


		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		VkBuffer vertexBuffers[] = { batchList[i]->vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(commandBuffer, batchList[i]->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		VkDescriptorSet sets[] = { batchList[i]->descriptorSets[currentFrame] , batchList[i]->descriptorSetsLight[currentFrame] };

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, batchList[i]->pipelineLayout, 0, 2, sets, 0, nullptr);

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(batchList[i]->indices.size()), 1, 0, 0, 0);

	}
	if (stateUI) {
		for (int i = 0; i < batchListText.size(); i++) {

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, batchListText[i]->graphicsPipeline);


			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);


			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

			VkBuffer vertexBuffers[] = { batchListText[i]->vertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(commandBuffer, batchListText[i]->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, batchListText[i]->pipelineLayout, 0, 1, &(batchListText[i]->descriptorSets[currentFrame]), 0, nullptr);

			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(batchListText[i]->indices.size()), 1, 0, 0, 0);


			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGuiWindow();

			ImGui::Render();

			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
		}
	}

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}


}

void RenderQueue::createSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(instance.device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(instance.device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(instance.device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

void RenderQueue::initImGui()
{
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;


	ImGui_ImplGlfw_InitForVulkan(instance.window, true);

	ImGui_ImplVulkan_InitInfo initInfo;
	initInfo.DescriptorPool = batchListText[0]->descriptorPool;
	initInfo.Instance = instance.instance;
	initInfo.Queue = instance.graphicsQueue;
	initInfo.QueueFamily = 0;
	initInfo.Subpass = 0;
	initInfo.CheckVkResultFn = check_vk_result;
	initInfo.PipelineCache = nullptr;
	initInfo.UseDynamicRendering = false;
	initInfo.Allocator = nullptr;
	initInfo.RenderPass = instance.renderpass;
	initInfo.Device = instance.device;
	initInfo.PhysicalDevice = instance.physicalDevice;
	initInfo.ImageCount = MAX_FRAMES_IN_FLIGHT;
	initInfo.MinImageCount = instance.minImageCount;
	initInfo.MSAASamples = batchListText[0]->msaaSamples;
	initInfo.MinAllocationSize = 1024*1024;
	ImGui_ImplVulkan_Init(&initInfo);


	ImGui_ImplVulkan_CreateFontsTexture();

	vkDeviceWaitIdle(instance.device);





}

// Callback function
int MyCallback(ImGuiInputTextCallbackData* data) {
	if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
		// Handle tab completion here
	}
	else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory) {
		// Handle history navigation here
	}
	std::cout << "hi" << std::endl;
	std::cout << data->BufTextLen << std::endl;

	std::cout << data->Buf << std::endl;
	return 0;
}

void RenderQueue::ImGuiWindow()
{

	static float sliderValue = 0.0f;

	static char buffer[128] = "Type here...";
	if (ImGui::Begin("Example Input")) {
		ImGui::InputText("Input with callback", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, MyCallback);
		for (int bt = 0; bt < batchList.size(); bt++) {
			for (int l = 0; l < batchList[bt]->lights.size(); l++) {
				std::string name = batchList[bt]->lights[l]->name + " Power ";
				if (ImGui::SliderFloat(name.c_str(), &sliderValue, 0.0f, 5000.0f)) {
					batchList[bt]->lights[l]->color = glm::vec4(batchList[bt]->lights[l]->color.x,
						batchList[bt]->lights[l]->color.y, batchList[bt]->lights[l]->color.z, sliderValue);
				}
			}
		}
		
		
	}
	
	ImGui::End();
}




VkCommandBuffer RenderQueue::beginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(instance.device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void RenderQueue::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(instance.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(instance.graphicsQueue);

	vkFreeCommandBuffers(instance.device, commandPool, 1, &commandBuffer);
}
