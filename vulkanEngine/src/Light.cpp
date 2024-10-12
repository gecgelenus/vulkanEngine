#include "Light.hpp"

Light::Light()
{
	name = "Undefined name";
	position = glm::vec4(1.0f);
	color = glm::vec4(1.0f);
	ID = 0;
}

Light::Light(const char* name)
{
	this->name = name;
	position = glm::vec4(1.0f);
	color = glm::vec4(1.0f);
	ID = 0;
}

Light::Light(const std::string& name)
{
	this->name = name;
	position = glm::vec4(1.0f);
	color = glm::vec4(1.0f);
	ID = 0;
}

Light::~Light()
{
}
