
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <filesystem>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <math.h>

#include <vulkan/vulkan.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

#include <chrono>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <optional>
#include <limits>
#include <algorithm>
#include <fstream>
#include "RenderQueue.hpp"
#include "Texture.hpp"

#include "Font.hpp"
#include <sstream>

class MainClass
{
public:
	void run()
	{

		auto path = std::filesystem::current_path(); // getting path
		std::filesystem::current_path(path / "..");	 // setting path

		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			printf("Current working dir: %s\n", cwd);
		}
		else
		{
			perror("getcwd() error");
			return;
		}

		initWindow();
		initVulkan();

		mainLoop();
	}

private:
	void mainLoop()
	{
		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

		VmaVulkanFunctions vulkanFunctions = {};
		vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
		vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

		VmaAllocatorCreateInfo allocatorCreateInfo = {};
		allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
		allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
		allocatorCreateInfo.physicalDevice = physicalDevice;
		allocatorCreateInfo.device = device;
		allocatorCreateInfo.instance = instance;
		allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

		VmaAllocator allocator;
		vmaCreateAllocator(&allocatorCreateInfo, &allocator);

		InstanceVariables vars{};

		vars.instance = instance;
		vars.device = device;
		vars.physicalDevice = physicalDevice;
		vars.graphicsQueue = graphicsQueue;
		vars.presentQueue = presentQueue;
		vars.window = window;
		vars.swapchain = swapChain;
		vars.swapchainFramebuffers = swapChainFramebuffers;
		vars.swapchainExtent = swapChainExtent;
		vars.surface = surface;
		vars.renderpass = renderPass;
		vars.minImageCount = minImage;
		vars.multiDrawSupport = multiDrawSupport;

		vars.WIDTH = WIDTH;
		vars.HEIGHT = HEIGHT;
		vars.allocator = allocator;

		RenderQueue renderQueue(vars);

		std::ifstream t("textures/fonts/sansa_32.fnt");
		std::stringstream buffer;
		buffer << t.rdbuf();

		FontData font;
		font.parse(buffer.str());

		// Output parsed data
		std::cout << "Font Face: " << font.face << "\n";
		std::cout << "Font Size: " << font.size << "\n";
		std::cout << "Number of Characters: " << font.chars.size() << "\n";
		std::cout << "Number of Kernings: " << font.kernings.size() << "\n";

		std::string texturePathText = "fonts/sansa_32_0.png";
		Texture *textureText = new Texture(allocator, device, commandPool, graphicsQueue, texturePathText, false);

		std::string batchName = "guiBatch";
		RenderBatchText *batchText = new RenderBatchText(batchName, vars, "shaders/vertText.spv", "shaders/fragText.spv");
		renderQueue.pushToQueue(batchText);

		RenderBatch *batch = new RenderBatch("Test Batch", vars, "shaders/vertFlat.spv", "shaders/fragFlatNonMaterial.spv");

		Object *obj1 = new Object("sphere1", "models/sphere.obj", batch->textureMap);
		Object *obj2 = new Object("sphere2", "models/sphere.obj", batch->textureMap);
		
		obj1->setColor(glm::vec4(1.0f));
		obj2->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		obj1->position = glm::vec3(0.0f);
		obj2->position = glm::vec3(3.0f);

		batch->addObject(obj1);
		batch->addObject(obj2);
		
		Light *light1 = new Light("light1");
		light1->position = glm::vec4(10.0f, 10.0f, 10.0f, 1.0f);
		light1->color = glm::vec4(1.0f, 1.0f, 1.0f, 100.0f);

		batch->addLight(light1);
		renderQueue.pushToQueue(batch);
		//saveStats(allocator);

		while (!glfwWindowShouldClose(window))
		{
			VkResult result = renderQueue.drawFrame();
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				vkDeviceWaitIdle(device);
				for (auto framebuffer : swapChainFramebuffers)
				{
					vkDestroyFramebuffer(device, framebuffer, nullptr);
				}

				for (auto imageView : swapChainImageViews)
				{
					vkDestroyImageView(device, imageView, nullptr);
				}

				vkDestroySwapchainKHR(device, swapChain, nullptr);

				createSwapChain();
				createImageViews();
				createFramebuffers();
				renderQueue.instance.swapchain = swapChain;
				renderQueue.instance.swapchainExtent = swapChainExtent;
				renderQueue.instance.swapchainFramebuffers = swapChainFramebuffers;
				std::cout << "Swapchain recreated" << std::endl;
			}

			glfwPollEvents();
		}

		vkDeviceWaitIdle(device);

		// -------------------------SWAPCHAIN CLEANUP--------------------------------

		vkDestroyImageView(device, colorImageView, nullptr);
		vkDestroyImage(device, colorImage, nullptr);
		vkFreeMemory(device, colorImageMemory, nullptr);

		vkDestroyImageView(device, depthImageView, nullptr);
		vkDestroyImage(device, depthImage, nullptr);
		vkFreeMemory(device, depthImageMemory, nullptr);

		for (auto framebuffer : swapChainFramebuffers)
		{
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}

		for (auto imageView : swapChainImageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(device, swapChain, nullptr);
	}

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "MyEngine", nullptr, nullptr);
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// position
		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	}

	void initVulkan()
	{
		createInstance();
		createSurface();
		pickPhysicalDevice(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createColorResources();
		createDepthResources();
		createFramebuffers();
		createCommandPool();
		createCommandBuffers();
	}

	void cleanup()
	{

		for (auto framebuffer : swapChainFramebuffers)
		{
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
		vkDestroyRenderPass(device, renderPass, nullptr);

		for (auto imageView : swapChainImageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(device, swapChain, nullptr);
		vkDestroyDevice(device, nullptr);
		vkDestroySurfaceKHR(instance, surface, nullptr);

		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}

	// VULKAN INITIALIZATION

	void createInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan Engine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Vulkan Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance!");
		}
	}

	void createSurface()
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void pickPhysicalDevice(VkPhysicalDeviceType type)
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		std::vector<uint32_t> scores;

		for (VkPhysicalDevice pDevice : devices)
		{
			scores.push_back(rateDevice(pDevice, type));
		}

		auto maxScore = std::max_element(scores.begin(), scores.end());

		physicalDevice = devices[std::distance(scores.begin(), maxScore)];

		if (physicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}

	void createLogicalDevice()
	{
		float queuePriority = 1.0f;
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		graphicsQueueIndex = getGraphicsQueueIndex(physicalDevice);
		computeQueueIndex = getComputeQueueIndex(physicalDevice);
		transferQueueIndex = getTransferQueueIndex(physicalDevice);

		if(graphicsQueueIndex == QUEUE_NOT_FOUND){
			std::cout << "Graphics queue not found!" << std::endl;
		}else{
			std::cout << "Graphics queue found; " << graphicsQueueIndex << std::endl;
			// Graphics queue
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		if(computeQueueIndex == QUEUE_NOT_FOUND){
			std::cout << "Compute queue not found!" << std::endl;
		}else{
			std::cout << "Compute queue found; " << computeQueueIndex << std::endl;
			// Compute queue
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = computeQueueIndex;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		if(transferQueueIndex == QUEUE_NOT_FOUND){
			std::cout << "Transfer queue not found!" << std::endl;
		}else{
			std::cout << "Transfer queue found; " << transferQueueIndex << std::endl;
			// Transfer queue
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = transferQueueIndex;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		
		

		

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.sampleRateShading = VK_TRUE;
		deviceFeatures.multiDrawIndirect = VK_TRUE;

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

		if (!supportedFeatures.multiDrawIndirect) {
    		std::cout << "MultiDrawIndirect is supported!" << std::endl;
			multiDrawSupport = VK_FALSE;
		}else{
			multiDrawSupport = VK_TRUE;
		}

		if (!supportedFeatures.sampleRateShading) {
			throw std::runtime_error("Sample rate shading is not supported on this device and I am too lazy to handle otherwise. Pick another GPU please.");
		}

		if (!supportedFeatures.samplerAnisotropy) {
			throw std::runtime_error("Sampler anisotropy is not supported on this device and I am too lazy to handle otherwise. Pick another GPU please.");
		}


		VkPhysicalDeviceVulkan12Features features12Check{};
		features12Check.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;

		VkPhysicalDeviceFeatures2 features2Check{};
		features2Check.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		features2Check.pNext = &features12Check;

		vkGetPhysicalDeviceFeatures2(physicalDevice, &features2Check);

		if (!features12Check.runtimeDescriptorArray) {
			throw std::runtime_error("Runtime descriptor arrays are not supported on this device and I am too lazy to handle otherwise. Pick another GPU please.");
		}

		VkPhysicalDeviceVulkan12Features vulkan12Features{};
		vulkan12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
		vulkan12Features.runtimeDescriptorArray = VK_TRUE; // Enable the feature



		VkPhysicalDeviceFeatures2 deviceFeatures2{};
		deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		deviceFeatures2.features = deviceFeatures; // Set traditional features here
		deviceFeatures2.pNext = &vulkan12Features; // Link Vulkan 1.2 features

		vkGetPhysicalDeviceFeatures2(physicalDevice, &deviceFeatures2);


		

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = VK_NULL_HANDLE;

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.pNext = &deviceFeatures2;

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create logical device!");
		}


		if(graphicsQueueIndex != QUEUE_NOT_FOUND){
			vkGetDeviceQueue(device, graphicsQueueIndex, 0, &graphicsQueue);
			vkGetDeviceQueue(device, graphicsQueueIndex, 0, &presentQueue);
		}

		if(computeQueueIndex != QUEUE_NOT_FOUND){
			vkGetDeviceQueue(device, computeQueueIndex, 0, &computeQueue);
		}

		if(transferQueueIndex != QUEUE_NOT_FOUND){
			vkGetDeviceQueue(device, transferQueueIndex, 0, &transferQueue);
		}


		
	}

	void createSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		minImage = swapChainSupport.capabilities.minImageCount;

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;	  // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;

		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
	}

	void createImageViews()
	{
		swapChainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create image views!");
			}
		}
	}

	void createRenderPass()
	{
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = msaaSamples;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = VK_FORMAT_D32_SFLOAT_S8_UINT;
		depthAttachment.samples = msaaSamples;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription colorAttachmentResolve{};
		colorAttachmentResolve.format = swapChainImageFormat;
		colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentDescription attachmentDescriptions[] = {colorAttachment, depthAttachment, colorAttachmentResolve};

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorAttachmentResolveRef{};
		colorAttachmentResolveRef.attachment = 2;
		colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;
		subpass.pResolveAttachments = &colorAttachmentResolveRef;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 3;
		renderPassInfo.pAttachments = attachmentDescriptions;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create render pass!");
		}
	}

	void createFramebuffers()
	{
		swapChainFramebuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); i++)
		{
			VkImageView attachments[] = {
				colorImageView,
				depthImageView,
				swapChainImageViews[i]};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 3;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	void createDepthResources()
	{
		createImage(swapChainExtent.width, swapChainExtent.height, msaaSamples, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);

		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = depthImage;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = VK_FORMAT_D32_SFLOAT_S8_UINT;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &createInfo, nullptr, &depthImageView) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image views!");
		}
	}

	void createColorResources()
	{
		VkFormat colorFormat = swapChainImageFormat;

		createImage(swapChainExtent.width, swapChainExtent.height, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = colorImage;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &createInfo, nullptr, &colorImageView) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image views!");
		}
	}
	void createCommandPool()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = 0;

		if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create command pool!");
		}
	}

	void createCommandBuffers()
	{
		commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}
	// HELPERS

	bool isDeviceSuitable(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			   deviceFeatures.geometryShader;
	}

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	void saveStats(VmaAllocator allocator)
	{
		char *statsString;

		// Get the stats string
		vmaBuildStatsString(allocator, &statsString, VK_TRUE); // VK_TRUE for detailed statistics

		std::cout << statsString << std::endl;

		std::fstream file;
		file.open("stats.txt");
		std::string stats(statsString);

		file.write(stats.data(), stats.length());
		file.close();

		// Free the allocated string by VMA
		vmaFreeStatsString(allocator, statsString);
	}

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	void createImage(uint32_t width, uint32_t height, VkSampleCountFlagBits numSample, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = numSample;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(device, image, imageMemory, 0);
	}

	// DEVICE SELECT

	uint32_t rateDevice(VkPhysicalDevice pDevice, VkPhysicalDeviceType preferredType)
	{

		uint32_t score = 0;

		std::string supportedArray[2];
		supportedArray[0] = "Not supported";
		supportedArray[1] = "Supported";

		uint32_t scoreArr[2];
		scoreArr[0] = 0;
		scoreArr[1] = 1;

		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(pDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(pDevice, &deviceFeatures);

		// Lets print device information so overall program looks cooler.
		std::cout << "--------------------------------------------------" << std::endl;
		std::cout << "Device found: " << deviceProperties.deviceName << std::endl;
		switch (deviceProperties.deviceType)
		{
		case 0:
			std::cout << "Device type: Other" << std::endl;
			break;
		case 1:
			std::cout << "Device type: Integrated GPU" << std::endl;
			if(deviceProperties.deviceType == preferredType){
				score += 10000; // Basically means selecting this device
			}else{
				score += 10;
			}
			break;
		case 2:
			std::cout << "Device type: Discrete GPU" << std::endl;
			if(deviceProperties.deviceType == preferredType){
				score += 10000; // Basically means selecting this device
			}else{
				score += 60;
			}
			break;
		case 3:
			std::cout << "Device type: Virtual GPU" << std::endl;
			if(deviceProperties.deviceType == preferredType){
				score += 10000; // Basically means selecting this device
			}else{
				score += 5;
			}
			break;
		case 4:
			std::cout << "Device type: Cpu (really?)" << std::endl;
			if(deviceProperties.deviceType == preferredType){
				score += 10000; // Basically means selecting this device
			}
			break;
		default:
			std::cout << "Device type: N/A" << std::endl;
			break;
		}

		switch (deviceProperties.vendorID)
		{
		case 0x1002:
			std::cout << "Vendor: AMD" << std::endl;
			break;
		case 0x1010:
			std::cout << "Vendor: ImgTec" << std::endl;
			break;
		case 0x10DE:
			std::cout << "Vendor: NVIDIA" << std::endl;
			break;
		case 0x13B5:
			std::cout << "Vendor: ARM" << std::endl;
			break;
		case 0x5143:
			std::cout << "Vendor: Qualcomm" << std::endl;
			break;
		case 0x8086:
			std::cout << "Vendor: Intel" << std::endl;
			break;
		default:
			std::cout << "Vendor: N/A" << std::endl;
			break;
		}

		std::cout << "Driver version: " << deviceProperties.driverVersion << std::endl;
		std::cout << "API version: " << deviceProperties.apiVersion << std::endl;

		// Device Features
		std::cout << "***Device Features***" << std::endl;

		std::cout << "MultiDrawIndirect: " << supportedArray[deviceFeatures.multiDrawIndirect] << std::endl;
		score += scoreArr[deviceFeatures.multiDrawIndirect] * 10;
		std::cout << "Max DrawIndirect count: " << deviceProperties.limits.maxDrawIndirectCount << std::endl;

		std::cout << "Depth clamp: " << supportedArray[deviceFeatures.depthClamp] << std::endl;
		score += scoreArr[deviceFeatures.depthClamp] * 10;

		std::cout << "Depth bias clamp: " << supportedArray[deviceFeatures.depthBiasClamp] << std::endl;
		score += scoreArr[deviceFeatures.depthBiasClamp] * 10;

		std::cout << "Geometry shader: " << supportedArray[deviceFeatures.geometryShader] << std::endl;

		std::cout << "Tessellation shader: " << supportedArray[deviceFeatures.tessellationShader] << std::endl;

		std::cout << "MultiViewport: " << supportedArray[deviceFeatures.multiViewport] << std::endl;

		std::cout << "AlphaToOne: " << supportedArray[deviceFeatures.alphaToOne] << std::endl;

		std::cout << "Robust buffer access: " << supportedArray[deviceFeatures.robustBufferAccess] << std::endl;

		std::cout << "Sparse binding: " << supportedArray[deviceFeatures.sparseBinding] << std::endl;

		std::cout << "Sampler anisotropy: " << supportedArray[deviceFeatures.samplerAnisotropy] << std::endl;
		std::cout << "Max sampler anisotropy: " << deviceProperties.limits.maxSamplerAnisotropy << std::endl;
		score += scoreArr[deviceFeatures.samplerAnisotropy] * deviceProperties.limits.maxSamplerAnisotropy;

		std::cout << "Sample rate shading: " << supportedArray[deviceFeatures.sampleRateShading] << std::endl;
		score += scoreArr[deviceFeatures.sampleRateShading] * 10;

		std::cout << "***Device Limits***" << std::endl;

		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(pDevice, &memoryProperties);

		VkDeviceSize vramSize = 0;

		for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; i++)
		{
			if (memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
			{
				vramSize = memoryProperties.memoryHeaps[i].size;
				break; // This is the dedicated VRAM, you can break out of the loop.
			}
		}

		std::cout << "Dedicated VRAM size: " << vramSize / (1024 * 1024) << " MB" << std::endl;

		std::cout << "Max push constants size: " << deviceProperties.limits.maxPushConstantsSize << std::endl;
		std::cout << "Max compute shared memory size: " << deviceProperties.limits.maxComputeSharedMemorySize << std::endl;
		std::cout << "Max memory allocation count: " << deviceProperties.limits.maxMemoryAllocationCount << std::endl;
		std::cout << "Max color attachments: " << deviceProperties.limits.maxColorAttachments << std::endl;
		std::cout << "Max bound descriptor sets: " << deviceProperties.limits.maxBoundDescriptorSets << std::endl;
		std::cout << "Max storage buffer range: " << deviceProperties.limits.maxStorageBufferRange << std::endl;
		std::cout << "Max drawIndexed index value: " << deviceProperties.limits.maxDrawIndexedIndexValue << std::endl;
		std::cout << "Max framebuffer layers: " << deviceProperties.limits.maxFramebufferLayers << std::endl;
		std::cout << "Max framebuffer width: " << deviceProperties.limits.maxFramebufferWidth << std::endl;
		std::cout << "Max framebuffer height: " << deviceProperties.limits.maxFramebufferHeight << std::endl;
		std::cout << "Max per stage resources: " << deviceProperties.limits.maxPerStageResources << std::endl;
		std::cout << "Max per stage input attachments: " << deviceProperties.limits.maxPerStageDescriptorInputAttachments << std::endl;
		std::cout << "Max per stage sampled images: " << deviceProperties.limits.maxPerStageDescriptorSampledImages << std::endl;
		std::cout << "Max per stage samplers: " << deviceProperties.limits.maxPerStageDescriptorSamplers << std::endl;
		std::cout << "Max per stage storage buffers: " << deviceProperties.limits.maxPerStageDescriptorStorageBuffers << std::endl;
		std::cout << "Max per stage storage images: " << deviceProperties.limits.maxPerStageDescriptorStorageImages << std::endl;
		std::cout << "Max per stage uniform buffers: " << deviceProperties.limits.maxPerStageDescriptorUniformBuffers << std::endl;

		std::cout << "***Device Extensions***" << std::endl;
		for (const char *ext : deviceExtensions)
		{
			std::cout << ext << std::endl;
		}

		// The device that is going to be used is determined by score of it.
		std::cout << "\n Overall device score: " << score << std::endl;
		std::cout << "--------------------------------------------------" << std::endl;

		return score;
	}

	// SWAPCHAIN

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
	{
		for (const auto &availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
	{
		for (const auto &availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	uint32_t getComputeQueueIndex(VkPhysicalDevice physicalDevice){
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> families(queueFamilyCount);

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, families.data());

		for(int i = 0; i < queueFamilyCount; i++){
			if((families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) && !(families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)){
				return i;
			}
		}
		



		return QUEUE_NOT_FOUND;
	}

	uint32_t getGraphicsQueueIndex(VkPhysicalDevice physicalDevice){
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> families(queueFamilyCount);

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, families.data());

		for(int i = 0; i < queueFamilyCount; i++){
			if((families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)){
				return i;
			}
		}
		



		return QUEUE_NOT_FOUND;
	}

	uint32_t getTransferQueueIndex(VkPhysicalDevice physicalDevice){
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> families(queueFamilyCount);

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, families.data());

		for(int i = 0; i < queueFamilyCount; i++){
			if(!(families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) && !(families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && (families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)){
				return i;
			}
		}
		



		return QUEUE_NOT_FOUND;
	}


	// WINDOW
	uint32_t WIDTH = 1600;
	uint32_t HEIGHT = 900;

	GLFWwindow *window;

	VkSurfaceKHR surface;
	uint32_t minImage;
	// INSTANCE
	VkInstance instance;

	uint32_t graphicsQueueIndex;
	uint32_t computeQueueIndex;
	uint32_t transferQueueIndex;


	// DEVICE
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkQueue computeQueue;
	VkQueue transferQueue;

	

	VkRenderPass renderPass;
	VkBool32 multiDrawSupport;

	// RENDER
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_8_BIT;

	// COMMAND

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	// DATA

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	// CONSTANTS
	const int MAX_FRAMES_IN_FLIGHT = 3;

	const std::vector<const char *> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	const std::vector<const char *> validationLayers = {
		"VK_LAYER_KHRONOS_validation"};
};

int main()
{
	MainClass app;

	try
	{
		app.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}