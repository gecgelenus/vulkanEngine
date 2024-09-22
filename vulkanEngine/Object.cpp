#include "Object.hpp"

Object::Object(std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	this->name = name;
	this->vertices = vertices;
	this->indices = indices;
	this->objectID = 0;
	this->offset = 0;
	this->properties = {};
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
}

Object::~Object()
{
}

void Object::setID(uint32_t id)
{
	this->objectID = id;

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].ID = objectID;
	}
}

uint32_t Object::getID()
{
	return objectID;
}