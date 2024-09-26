#pragma once
#include "Util.hpp"
#include <vector>
#include <string>



class Object {
public:
	Object();
	Object(std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	Object(const char* name, std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	Object(const char* name, const char* path);

	~Object();


	void setID(uint32_t id);
	void setMaterialOffset();

	void setColor(const glm::vec4& color);
	void loadOBJ(const char* path);
	uint32_t getID();
	std::string name;
	
	glm::vec3 position;
	glm::vec3 color;
	
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Material> materials;

	
	uint32_t offset;
	uint32_t materialOffset;


	objectProperties properties;

protected:
	uint32_t objectID;
	


};