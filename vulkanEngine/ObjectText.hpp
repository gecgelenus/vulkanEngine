#pragma once
#include "Util.hpp"
#include <vector>
#include <string>

#include "Object.hpp"
#include "Font.hpp"
#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb_truetype.h" 

class ObjectText: public Object {

public:
	ObjectText(std::string name, std::vector<Vertex2D> vertices, std::vector<uint32_t> indices);
	ObjectText(std::string name, FontData& font);

	void setID(uint32_t id);
	void setColor(glm::vec3 color);

	
	void setText(std::string& text, float xPos, float yPos);
	void setText(std::string& text);
	void setPosition(float xPos, float yPos);
	void generateTextVertices(const std::string& text, float xPos, float yPos, float scale, FontData& font, glm::vec3 color);


	FontData font;
	std::string text;
	float xPos;
	float yPos;

	std::vector<Vertex2D> vertices;
	glm::vec3 color;
	uint32_t fontSize;

};