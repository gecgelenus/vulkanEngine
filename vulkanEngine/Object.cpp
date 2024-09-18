#include "Object.hpp"

Object::Object(std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	this->name = name;
	this->vertices = vertices;
	this->indices = indices;

}

Object::~Object()
{
}
