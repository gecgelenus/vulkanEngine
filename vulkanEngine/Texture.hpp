#pragma once

#define VMA_IMPLEMENTATION
#include "vulkan_mem_alloc.h"


#include <vector>


class Texture {

public:
	Texture(VmaAllocator allocator, VkDevice device, VkCommandPool commandPool, VkQueue queue, std::string& path);
	~Texture();

	VkImage getImage() { return textureImage; };
	VkImageView getImageView() { return textureImageView; };

	uint32_t textureID;



private:

	VmaAllocator allocator;

	VkImage textureImage;
	VkImageView textureImageView;

	VkDevice device;
	VkCommandPool commandPool;
	VkQueue queue;

	VmaAllocation imageAllocation;


	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

};