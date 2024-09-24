#include "Light.hpp"

Light::Light()
{
	position = glm::vec4(1.0f);
	color = glm::vec4(1.0f);
	ID = 0;
}

Light::~Light()
{
}
