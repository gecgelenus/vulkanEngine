#include "Object.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <stdexcept>
#include <iostream>


Object::Object()
{
}

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

Object::Object(const char* name, std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	this->name = name;
	this->vertices = vertices;
	this->indices = indices;
	this->objectID = 0;
	this->offset = 0;
	this->properties = {};
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
}

Object::Object(const char* name, const char* path)
{
	this->name = name;
	this->objectID = 0;
	this->offset = 0;
	this->properties = {};
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);

	loadOBJ(path);
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

void Object::setColor(const glm::vec3& color)
{
	this->color = color;

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].color = color;
	}
}

void Object::loadOBJ(const char* path)
{
	vertices.clear();
	indices.clear();


	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./"; // Path to material files

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(path, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();


	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				Vertex tmpVertex{};

				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tmpVertex.pos.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tmpVertex.pos.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tmpVertex.pos.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					tmpVertex.normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tmpVertex.normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tmpVertex.normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					tmpVertex.texCoord.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tmpVertex.texCoord.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}

				// Optional: vertex colors
				// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
				tmpVertex.ID = 1;
				vertices.push_back(tmpVertex);

				// Store the vertex index in the index buffer
				indices.push_back(static_cast<uint32_t>(indices.size()));
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
}

uint32_t Object::getID()
{
	return objectID;
}