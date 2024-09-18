#pragma once
#include "Util.hpp"
#include <vector>
#include <string>



class Object {
public:
	Object(std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	~Object();

	
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;





};