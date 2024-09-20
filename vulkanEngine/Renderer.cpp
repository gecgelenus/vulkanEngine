#include "Renderer.hpp"
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Renderer::Renderer()
{
	position = glm::vec3(5, 5, 5);
	// horizontal angle : toward -Z
	horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	verticalAngle = 0.0f;

	speed = 3.0f; // 3 units / second
	mouseSpeed = 1.0f;
}

Renderer::~Renderer()
{
	

	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		vkDestroyBuffer(device, objectPropertyBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
		vkFreeMemory(device, objectPropertyBuffersMemory[i], nullptr);

	}

	vkDestroyDescriptorPool(device, descriptorPool, nullptr);

	vkDestroySampler(device, textureSampler, nullptr);

	

	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

	vkDestroyBuffer(device, indexBuffer, nullptr);
	vkFreeMemory(device, indexBufferMemory, nullptr);

	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(device, vertexBufferMemory, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(device, commandPool, nullptr);

	vkDestroyDevice(device, nullptr);

	

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();

}


void Renderer::drawFrame()
{
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);


	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	VkSwapchainKHR swapChains[] = { swapChain };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	updateUniformBuffer(imageIndex);


	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
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

	vkQueuePresentKHR(presentQueue, &presentInfo);
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;


}

void Renderer::addObject(Object* obj)
{
	
	uint32_t offset = this->vertices.size();
	obj->offset = offset;
	obj->setID(objects.size());
	obj->properties.objectID = objects.size();
	std::cout << "ID: " << obj->getID() << std::endl;

		this->vertices.insert(this->vertices.end(), obj->vertices.begin(), obj->vertices.end());
	
	for (uint32_t i : obj->indices) {
		this->indices.push_back(i + offset);
	}
	
	this->objects.push_back(obj);

	// VERTEX

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);

	// INDEX

	bufferSize = sizeof(indices[0]) * indices.size();

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);
	void* data2;

	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data2);
	memcpy(data2, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);


	/*
	* 
	for (Vertex v : vertices) {
		std::cout << v.pos.x << " " << v.pos.y << " " << v.pos.z << " " << v.normal.x << " " << v.normal.y << " " << v.normal.z << " " << v.texCoord.x << " " << v.texCoord.y << std::endl;
	}

	for (uint32_t i : indices) {
		std::cout << i << std::endl;
	}

	*/

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkResetCommandBuffer(commandBuffers[i], 0);
		recordCommandBuffer(commandBuffers[i], i);
	}


	std::cout << "Object is added to renderer: " << obj->name << std::endl;

	

}

void Renderer::deleteObject(std::string& name)
{
	Object* obj = getObject(name);

	objects.erase(objects.begin() + obj->getID());

	resetBuffers();



}

Object* Renderer::getObject(std::string& name)
{

	for (Object* obj : objects) {
		if (obj->name == name) {
			return obj;
		}
	}

	return nullptr;
}

void Renderer::setObjectTexture(std::string& name, Texture* texture)
{

	Object* obj = getObject(name);
	obj->properties.textureID = texture->textureID;

}

void Renderer::readOBJ(std::string& path)
{

	
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./"; // Path to material files

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(path, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();





	std::vector<Object*> objectArr;

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {

		std::vector<Vertex> tmpVertices;
		std::vector<uint32_t> tmpIndices;
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				Vertex tmpVertex{};

				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tmpVertex.pos.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tmpVertex.pos.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tmpVertex.pos.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					tmpVertex.normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tmpVertex.normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tmpVertex.normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					tmpVertex.texCoord.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tmpVertex.texCoord.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}

				// Optional: vertex colors
				// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
				tmpVertex.ID = 1;
				tmpVertices.push_back(tmpVertex);


				// Store the vertex index in the index buffer
				tmpIndices.push_back(static_cast<uint32_t>(tmpIndices.size()));

			}
			index_offset += fv;


			// per-face material
			shapes[s].mesh.material_ids[f];
		}
		objectArr.push_back(new Object(shapes[s].name, tmpVertices, tmpIndices));

	}

	for (Object* obj : objectArr) {
		addObject(obj);
	}

}

void Renderer::createVertexBuffer()
{
	VkDeviceSize bufferSize = STORAGE_MB * 500;
	
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer, vertexBufferMemory);


}

void Renderer::createIndexBuffer()
{
	VkDeviceSize bufferSize = STORAGE_MB * 100;

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	
}

void Renderer::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

		vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}

	bufferSize = sizeof(objectProperties) * OBJECT_COUNT;

	objectPropertyBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	objectPropertyBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	objectPropertyBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, objectPropertyBuffers[i], objectPropertyBuffersMemory[i]);

		vkMapMemory(device, objectPropertyBuffersMemory[i], 0, bufferSize, 0, &objectPropertyBuffersMapped[i]);
	}


	

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorBufferInfo propertybufferInfo{};
		propertybufferInfo.buffer = objectPropertyBuffers[i];
		propertybufferInfo.offset = 0;
		propertybufferInfo.range = sizeof(objectProperties) * OBJECT_COUNT;

		
		

		VkDescriptorImageInfo samplerInfo{};
		samplerInfo.sampler = textureSampler;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		VkWriteDescriptorSet descriptorWriteSampler{};
		descriptorWriteSampler.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteSampler.dstSet = descriptorSets[i];
		descriptorWriteSampler.dstBinding = 1;
		descriptorWriteSampler.dstArrayElement = 0;
		descriptorWriteSampler.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		descriptorWriteSampler.descriptorCount = 1;
		descriptorWriteSampler.pImageInfo = &samplerInfo;
		descriptorWriteSampler.pTexelBufferView = nullptr; // Optional

		

		VkWriteDescriptorSet descriptorWriteProperty{};
		descriptorWriteProperty.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteProperty.dstSet = descriptorSets[i];
		descriptorWriteProperty.dstBinding = 3;
		descriptorWriteProperty.dstArrayElement = 0;
		descriptorWriteProperty.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWriteProperty.descriptorCount = 1;
		descriptorWriteProperty.pBufferInfo = &propertybufferInfo;
		descriptorWriteProperty.pImageInfo = nullptr; // Optional
		descriptorWriteProperty.pTexelBufferView = nullptr; // Optional


		VkWriteDescriptorSet sets[] = { descriptorWrite, descriptorWriteSampler, descriptorWriteProperty };

		vkUpdateDescriptorSets(device, 3, sets, 0, nullptr);
	}


}

void Renderer::addTexture(Texture* texture)
{

	textures.push_back(texture);
	texture->textureID = textureViews.size();
	textureViews.push_back(texture->getImageView());

	updateTextureDescriptors();


}




void Renderer::createDescriptorPool()
{
	VkDescriptorPoolSize uniformPool{};
	uniformPool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uniformPool.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolSize texturePool{};
	texturePool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	texturePool.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolSize samplerPool{};
	samplerPool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	samplerPool.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolSize objectPropertyPool{};
	objectPropertyPool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	objectPropertyPool.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolSize sizes[] = { uniformPool, texturePool, samplerPool, objectPropertyPool };

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 4;
	poolInfo.pPoolSizes = sizes;
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void Renderer::createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding textureBinding{};
	textureBinding.binding = 2;
	textureBinding.descriptorCount = 64;
	textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	textureBinding.pImmutableSamplers = nullptr;
	textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding objectPropertyBinding{};
	objectPropertyBinding.binding = 3;
	objectPropertyBinding.descriptorCount = 1;
	objectPropertyBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	objectPropertyBinding.pImmutableSamplers = nullptr;
	objectPropertyBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding bindings[] = { uboLayoutBinding , samplerLayoutBinding, textureBinding, objectPropertyBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 4;
	layoutInfo.pBindings = bindings;

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void Renderer::allocateDescriptorSets()
{

	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

void Renderer::createTextureSampler()
{
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(pDevice, &properties);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;

	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void Renderer::updateUniformBuffer(uint32_t targetFrame)
{
	std::vector<objectProperties> propertyArray(1000);


	float currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos);

	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	horizontalAngle += mouseSpeed * deltaTime * float(WIDTH / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(HEIGHT / 2 - ypos);

	const float maxVerticalAngle = glm::radians(89.0f);
	if (verticalAngle > maxVerticalAngle)
		verticalAngle = maxVerticalAngle;
	else if (verticalAngle < -maxVerticalAngle)
		verticalAngle = -maxVerticalAngle;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
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

	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}
	// Move up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_REPEAT) {
		position += up * deltaTime * speed;
	}
	// Move down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_REPEAT) {
		position -= up * deltaTime * speed;
	}
	// Terminate program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}




	for (Object* obj : objects) {
		propertyArray[obj->getID()] = obj->properties;
	}

	for (Object* obj : objects) {
		ubo.model[propertyArray[obj->getID()].objectID] = glm::translate(glm::mat4(1.0f), obj->position);
	}

	ubo.view = glm::lookAt(position, position + direction, up);

	ubo.proj = glm::perspective(glm::radians(45.0f), WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	ubo.proj[1][1] *= -1;

	memcpy(uniformBuffersMapped[targetFrame], &ubo, sizeof(ubo));

	

	

	memcpy(objectPropertyBuffersMapped[targetFrame], propertyArray.data(), sizeof(objectProperties)* OBJECT_COUNT);



}

void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t index)
{
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	VkClearValue depthColor{};
	depthColor.depthStencil = { 1.0f, 0 };

	VkClearValue clearColorArray[] = { clearColor, depthColor };

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffers[index];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;
	renderPassInfo.clearValueCount = 2;
	renderPassInfo.pClearValues = clearColorArray;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void Renderer::createSyncObjects()
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
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

void Renderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

uint32_t Renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(pDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void Renderer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void Renderer::resetBuffers()
{
	vertices.clear();
	indices.clear();


	for (int i = 0; i < objects.size(); i++) {
		uint32_t offset = this->vertices.size();
		objects[i]->offset = offset;
		objects[i]->setID(i);
		objects[i]->properties.objectID = i;
		std::cout << "ID: " << objects[i]->getID() << std::endl;

		this->vertices.insert(this->vertices.end(), objects[i]->vertices.begin(), objects[i]->vertices.end());

		for (uint32_t i : objects[i]->indices) {
			this->indices.push_back(i + offset);
		}

	}


	// VERTEX

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);

	// INDEX

	bufferSize = sizeof(indices[0]) * indices.size();

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);
	void* data2;

	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data2);
	memcpy(data2, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);




	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkResetCommandBuffer(commandBuffers[i], 0);
		recordCommandBuffer(commandBuffers[i], i);
	}


}

void Renderer::updateTextureDescriptors()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		std::vector<VkDescriptorImageInfo> descriptorInfos;



		for (int i = 0; i < textureViews.size(); i++) {
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = textureViews[i];

			descriptorInfos.push_back(imageInfo);
		}

		VkWriteDescriptorSet descriptorWriteTex{};
		descriptorWriteTex.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteTex.dstSet = descriptorSets[i];
		descriptorWriteTex.dstBinding = 2;
		descriptorWriteTex.dstArrayElement = 0;
		descriptorWriteTex.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		descriptorWriteTex.descriptorCount = descriptorInfos.size();
		descriptorWriteTex.pImageInfo = descriptorInfos.data();
		descriptorWriteTex.pTexelBufferView = nullptr; // Optional



		vkUpdateDescriptorSets(device, 1, &descriptorWriteTex, 0, nullptr);
	}


}

void Renderer::deleteTexture(Texture* texture)
{
	//TODO


}
