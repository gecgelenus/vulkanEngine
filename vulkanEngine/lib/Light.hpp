#pragma once


#include <glm/glm.hpp>
#include <string>

class Light{

public:

	Light();
	Light(const char* name);
	Light(const std::string& name);

	~Light();


	std::string name;
	glm::vec4 position;
	glm::vec4 color;
	uint32_t ID;


};