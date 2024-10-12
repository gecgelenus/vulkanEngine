#include "ObjectText.hpp"
#include <iostream>

ObjectText::ObjectText(std::string name, std::vector<Vertex2D> vertices, std::vector<uint32_t> indices)
{
	this->name = name;
	this->vertices = vertices;
	this->indices = indices;
	this->objectID = 0;
	this->offset = 0;
	this->properties = {};
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->color = glm::vec3(1.0f, 1.0f, 1.0f);


}

ObjectText::ObjectText(std::string name, FontData& font)
{
	this->font = font;
	this->name = name;
	this->objectID = 0;
	this->offset = 0;
	this->properties = {};
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->color = glm::vec3(1.0f, 1.0f, 1.0f);

}

void ObjectText::setID(uint32_t id)
{
	this->objectID = id;

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].ID = objectID;
	}
}

void ObjectText::setColor(glm::vec3 color)
{
	this->color = color;

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].color = color;
	}
}


void ObjectText::setText(std::string& text, float xPosP, float yPosP)
{
	this->vertices.clear();
	this->indices.clear();
	
	this->xPos = xPosP;
	this->yPos = SCREEN_HEIGHT - yPosP;
	this->text = text;

	float xOffset = 0;
	int indexOffset = 0;


	int prev = 0;

	for (int i = 0; i < text.size(); i++) {

		Vertex2D v1;
		Vertex2D v2;
		Vertex2D v3;
		Vertex2D v4;

		char c = text[i];
		CharInfo currentChar{};
		for (int j = 0; j < font.chars.size(); j++) {
			if (font.chars[j].id == int(c)) {
				currentChar = font.chars[j];
				break;
			}
		}
		for (Kerning k : font.kernings) {
			if (0 == prev) {
				break;
			}
			else if (k.first == prev && k.second == currentChar.id) {
				xOffset += k.amount;
				break;

			}
		}


		float x0 = xOffset + xPos;
		float y0 = yPos;

		float x1 = xOffset + xPos + currentChar.width;
		float y1 = yPos - currentChar.height - currentChar.yoffset;




		v1.pos.x = 2.0f * (x0 / 1600) - 1.0f;
		v1.pos.y = 1.0f - 2.0f * (y0 / 900);
		v1.texCoord.x = currentChar.x / font.scale;
		v1.texCoord.y = currentChar.y / font.scale;
		v1.color = color;

		v2.pos.x = 2.0f * (x1 / 1600) - 1.0f;
		v2.pos.y = 1.0f - 2.0f * (y0 / 900);
		v2.texCoord.x = (currentChar.x + currentChar.width) / font.scale;
		v2.texCoord.y = currentChar.y / font.scale;
		v2.color = color;

		v3.pos.x = 2.0f * (x1 / 1600) - 1.0f;
		v3.pos.y = 1.0f - 2.0f * (y1 / 900);
		v3.texCoord.x = (currentChar.x + currentChar.width) / font.scale;
		v3.texCoord.y = (currentChar.y + currentChar.height) / font.scale;
		v3.color = color;

		v4.pos.x = 2.0f * (x0 / 1600) - 1.0f;
		v4.pos.y = 1.0f - 2.0f * (y1 / 900);
		v4.texCoord.x = currentChar.x / font.scale;
		v4.texCoord.y = (currentChar.y + currentChar.height) / font.scale;
		v4.color = color;

		this->vertices.push_back(v1);
		this->vertices.push_back(v2);
		this->vertices.push_back(v3);
		this->vertices.push_back(v4);

		this->indices.push_back(uint32_t(0 + indexOffset));
		this->indices.push_back(uint32_t(3 + indexOffset));
		this->indices.push_back(uint32_t(2 + indexOffset));
		this->indices.push_back(uint32_t(1 + indexOffset));
		this->indices.push_back(uint32_t(0 + indexOffset));
		this->indices.push_back(uint32_t(2 + indexOffset));


		xOffset += currentChar.xadvance;
		indexOffset += 4;
		prev = currentChar.id;
	}



}

void ObjectText::setText(std::string& text)
{
	vertices.clear();
	indices.clear();

	this->text = text;

	float xOffset = 0;
	int indexOffset = 0;


	int prev = 0;

	for (int i = 0; i < text.size(); i++) {

		Vertex2D v1;
		Vertex2D v2;
		Vertex2D v3;
		Vertex2D v4;

		char c = text[i];
		CharInfo currentChar{};
		for (int j = 0; j < font.chars.size(); j++) {
			if (font.chars[j].id == int(c)) {
				currentChar = font.chars[j];
				break;
			}
		}
		for (Kerning k : font.kernings) {
			if (0 == prev) {
				break;
			}
			else if (k.first == prev && k.second == currentChar.id) {
				xOffset += k.amount;
				break;

			}
		}


		float x0 = xOffset + xPos;
		float y0 = yPos;

		float x1 = xOffset + xPos + currentChar.width;
		float y1 = yPos - currentChar.height - currentChar.yoffset;




		v1.pos.x = 2.0f * (x0 / 1600) - 1.0f;
		v1.pos.y = 1.0f - 2.0f * (y0 / 900);
		v1.texCoord.x = currentChar.x / font.scale;
		v1.texCoord.y = currentChar.y / font.scale;
		v1.color = color;

		v2.pos.x = 2.0f * (x1 / 1600) - 1.0f;
		v2.pos.y = 1.0f - 2.0f * (y0 / 900);
		v2.texCoord.x = (currentChar.x + currentChar.width) / font.scale;
		v2.texCoord.y = currentChar.y / font.scale;
		v2.color = color;

		v3.pos.x = 2.0f * (x1 / 1600) - 1.0f;
		v3.pos.y = 1.0f - 2.0f * (y1 / 900);
		v3.texCoord.x = (currentChar.x + currentChar.width) / font.scale;
		v3.texCoord.y = (currentChar.y + currentChar.height) / font.scale;
		v3.color = color;

		v4.pos.x = 2.0f * (x0 / 1600) - 1.0f;
		v4.pos.y = 1.0f - 2.0f * (y1 / 900);
		v4.texCoord.x = currentChar.x / font.scale;
		v4.texCoord.y = (currentChar.y + currentChar.height) / font.scale;
		v4.color = color;

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);

		indices.push_back(uint32_t(0 + indexOffset));
		indices.push_back(uint32_t(3 + indexOffset));
		indices.push_back(uint32_t(2 + indexOffset));
		indices.push_back(uint32_t(1 + indexOffset));
		indices.push_back(uint32_t(0 + indexOffset));
		indices.push_back(uint32_t(2 + indexOffset));


		xOffset += currentChar.xadvance;
		indexOffset += 4;
		prev = currentChar.id;
	}

}

void ObjectText::setPosition(float xPosP, float yPosP)
{
	vertices.clear();
	indices.clear();

	this->xPos = xPosP;
	this->yPos = SCREEN_HEIGHT - yPosP;

	float xOffset = 0;
	int indexOffset = 0;


	int prev = 0;

	for (int i = 0; i < text.size(); i++) {

		Vertex2D v1;
		Vertex2D v2;
		Vertex2D v3;
		Vertex2D v4;

		char c = text[i];
		CharInfo currentChar{};
		for (int j = 0; j < font.chars.size(); j++) {
			if (font.chars[j].id == int(c)) {
				currentChar = font.chars[j];
				break;
			}
		}
		for (Kerning k : font.kernings) {
			if (0 == prev) {
				break;
			}
			else if (k.first == prev && k.second == currentChar.id) {
				xOffset += k.amount;
				std::cout << prev << " " << currentChar.id << " " << k.amount << std::endl;
				break;

			}
		}


		float x0 = xOffset + xPos;
		float y0 = yPos;

		float x1 = xOffset + xPos + currentChar.width;
		float y1 = yPos - currentChar.height - currentChar.yoffset;




		v1.pos.x = 2.0f * (x0 / 1600) - 1.0f;
		v1.pos.y = 1.0f - 2.0f * (y0 / 900);
		v1.texCoord.x = currentChar.x / font.scale;
		v1.texCoord.y = currentChar.y / font.scale;
		v1.color = color;

		v2.pos.x = 2.0f * (x1 / 1600) - 1.0f;
		v2.pos.y = 1.0f - 2.0f * (y0 / 900);
		v2.texCoord.x = (currentChar.x + currentChar.width) / font.scale;
		v2.texCoord.y = currentChar.y / font.scale;
		v2.color = color;

		v3.pos.x = 2.0f * (x1 / 1600) - 1.0f;
		v3.pos.y = 1.0f - 2.0f * (y1 / 900);
		v3.texCoord.x = (currentChar.x + currentChar.width) / font.scale;
		v3.texCoord.y = (currentChar.y + currentChar.height) / font.scale;
		v3.color = color;

		v4.pos.x = 2.0f * (x0 / 1600) - 1.0f;
		v4.pos.y = 1.0f - 2.0f * (y1 / 900);
		v4.texCoord.x = currentChar.x / font.scale;
		v4.texCoord.y = (currentChar.y + currentChar.height) / font.scale;
		v4.color = color;

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);

		indices.push_back(uint32_t(0 + indexOffset));
		indices.push_back(uint32_t(3 + indexOffset));
		indices.push_back(uint32_t(2 + indexOffset));
		indices.push_back(uint32_t(1 + indexOffset));
		indices.push_back(uint32_t(0 + indexOffset));
		indices.push_back(uint32_t(2 + indexOffset));


		xOffset += currentChar.xadvance;
		indexOffset += 4;
		prev = currentChar.id;
	}
}

void ObjectText::generateTextVertices(const std::string& text, float xPosP, float yPosP, float scale, FontData& font, glm::vec3 color)
{

	vertices.clear();
	indices.clear();

	float xOffset = 0;
	int indexOffset = 0;

	
	int prev = 0;

	for (int i = 0; i < text.size(); i++) {

		Vertex2D v1;
		Vertex2D v2;
		Vertex2D v3;
		Vertex2D v4;

		char c = text[i];
		CharInfo currentChar{};
		for (int j = 0; j < font.chars.size(); j++) {
			if (font.chars[j].id == int(c)) {
				currentChar = font.chars[j];
				break;
			}
		}
		for (Kerning k : font.kernings) {
			if (0 == prev) {
				break;
			}
			else if (k.first == prev && k.second == currentChar.id) {
				xOffset += k.amount;
				std::cout << prev << " " << currentChar.id << " " << k.amount << std::endl;
				break;

			}
		}


		float x0 = xOffset + xPos;
		float y0 = SCREEN_HEIGHT - yPosP;

		float x1 = xOffset + xPos + currentChar.width;
		float y1 = SCREEN_HEIGHT - yPosP - currentChar.height - currentChar.yoffset;




		v1.pos.x = 2.0f * (x0 / 1600) - 1.0f;
		v1.pos.y = 1.0f - 2.0f * (y0 / 900);
		v1.texCoord.x = currentChar.x / font.scale;
		v1.texCoord.y = currentChar.y / font.scale;

		v2.pos.x = 2.0f * (x1 / 1600) - 1.0f;
		v2.pos.y = 1.0f - 2.0f * (y0 / 900);
		v2.texCoord.x = (currentChar.x + currentChar.width) / font.scale;
		v2.texCoord.y = currentChar.y / font.scale;

		v3.pos.x = 2.0f * (x1 / 1600) - 1.0f;
		v3.pos.y = 1.0f - 2.0f * (y1 / 900);
		v3.texCoord.x = (currentChar.x + currentChar.width) / font.scale;
		v3.texCoord.y = (currentChar.y + currentChar.height) / font.scale;

		v4.pos.x = 2.0f * (x0 / 1600) - 1.0f;
		v4.pos.y = 1.0f - 2.0f * (y1 / 900);
		v4.texCoord.x = currentChar.x / font.scale;
		v4.texCoord.y = (currentChar.y + currentChar.height) / font.scale;

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);

		indices.push_back(uint32_t(0 + indexOffset));
		indices.push_back(uint32_t(3 + indexOffset));
		indices.push_back(uint32_t(2 + indexOffset));
		indices.push_back(uint32_t(1 + indexOffset));
		indices.push_back(uint32_t(0 + indexOffset));
		indices.push_back(uint32_t(2 + indexOffset));


		xOffset += currentChar.xadvance;
		indexOffset += 4;
		prev = currentChar.id;
	}
}
/*
vertices.push_back(Vertex2D{ {ndc_xpos,     ndc_ypos},     color, {u0, v1}, (uint32_t)ch });
vertices.push_back(Vertex2D{ {ndc_xpos_w,   ndc_ypos},     color, {u1, v1}, (uint32_t)ch });

vertices.push_back(Vertex2D{ {ndc_xpos_w,   ndc_ypos_h},   color, {u1, v0}, (uint32_t)ch });
vertices.push_back(Vertex2D{ {ndc_xpos,     ndc_ypos_h},   color, {u0, v0}, (uint32_t)ch });



indices.push_back(uint32_t(3 + indexOffset));
indices.push_back(uint32_t(0 + indexOffset));
indices.push_back(uint32_t(1 + indexOffset));
indices.push_back(uint32_t(2 + indexOffset));
indices.push_back(uint32_t(3 + indexOffset));
indices.push_back(uint32_t(1 + indexOffset));
indexOffset += 4;
*/