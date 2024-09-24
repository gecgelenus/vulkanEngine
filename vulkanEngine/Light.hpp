#pragma once


#include <glm/glm.hpp>


class Light{

public:

	Light();
	~Light();



	glm::vec4 position;
	glm::vec4 color;
	uint32_t ID;


};