#pragma once
#include "Util.hpp"
#include <vector>
#include <string>
#include <unordered_map>


class Object {
public:
	Object();
	Object(std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	Object(const char* name, std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	Object(const char* name, const char* path, std::unordered_map<std::string, int>& textureMap);

	~Object();


	void setID(uint32_t id);
	void setMaterialOffset();

	void updateMatrix();

	void setColor(const glm::vec4& color);
	void loadOBJ(const char* path);
	uint32_t getID();
	std::string name;
	
	glm::vec3 position;
	glm::vec3 color;
	
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Material> materials;
	std::unordered_map<std::string, int> textureMap;

	
	VkDeviceSize memoryOffsetVertex;
	VkDeviceSize memoryOffsetIndex;
	VmaVirtualAllocation virtualVertexAllocation;
	VmaVirtualAllocation virtualIndexAllocation;

	VkDrawIndexedIndirectCommand drawCommand {};

	


	
	uint32_t offset;
	uint32_t indexOffset;
	uint32_t materialOffset;
	float scale;
	float rotationX;
	float rotationY;
	float rotationZ;

	glm::mat4 modelMatrix;



	objectProperties properties;

protected:
	uint32_t objectID;
	


};