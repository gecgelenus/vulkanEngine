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
	void setColor(const glm::vec3& color);
	void loadOBJ(const char* path);
	uint32_t getID();
	std::string name;
	
	glm::vec3 position;
	glm::vec3 color;
	
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	
	
	uint32_t offset;

	objectProperties properties;

protected:
	uint32_t objectID;


};