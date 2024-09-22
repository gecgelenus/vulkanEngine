#pragma once
#include "Util.hpp"
#include <vector>
#include <string>

#include "Object.hpp"

class ObjectText: public Object {

public:
	ObjectText(std::string name, std::vector<Vertex2D> vertices, std::vector<uint32_t> indices);
	void setID(uint32_t id);



	std::vector<Vertex2D> vertices;


};