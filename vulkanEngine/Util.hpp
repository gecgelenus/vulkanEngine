#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define OBJECT_COUNT 1000

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoord;
};


struct objectProperties {
	glm::uint objectID;
	glm::uint materialID;
	glm::uint textureID;
	glm::uint normalMapID;

};
