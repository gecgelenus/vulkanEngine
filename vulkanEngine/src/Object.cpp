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
	this->materialOffset = 0;
	this->modelMatrix = glm::mat4(1.0f);
	this->scale = 1.0f;

	this->properties = {};
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
}

Object::Object(const char* name, std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	this->name = name;
	this->vertices = vertices;
	this->indices = indices;
	this->objectID = 0;
	this->materialOffset = 0;
	this->modelMatrix = glm::mat4(1.0f);
	this->scale = 1.0f;
	this->offset = 0;
	this->properties = {};
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
}

Object::Object(const char* name, const char* path, std::unordered_map<std::string, int>& textureMap)
{
	this->name = name;
	this->objectID = 0;
	this->offset = 0;
	this->properties = {};
	this->materialOffset = 0;
	this->scale = 1.0f;
	this->textureMap = textureMap;

	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->modelMatrix = glm::mat4(1.0f);
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

void Object::setMaterialOffset()
{

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].materialID += materialOffset;
	}
}

void Object::updateMatrix()
{
	modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, position);

	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationX), glm::vec3(1, 0, 0));
	
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationY), glm::vec3(0, 1, 0));
	
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationZ), glm::vec3(0, 0, 1));


}

void Object::setColor(const glm::vec4& color)
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
	reader_config.mtl_search_path = "./models/"; // Path to material files

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
	auto& objMaterials = reader.GetMaterials();

	uint32_t vertexCount = 0;

	

	this->materials.reserve(100);

	// Load materials
	for (const auto& mat : objMaterials) {
		Material tmpMaterial;
		tmpMaterial.ambient = { mat.ambient[0], mat.ambient[1], mat.ambient[2] };
		tmpMaterial.diffuse = { mat.diffuse[0], mat.diffuse[1], mat.diffuse[2] };
		tmpMaterial.specular = { mat.specular[0], mat.specular[1], mat.specular[2] };
		tmpMaterial.shininess = mat.shininess;
		tmpMaterial.transparency = mat.dissolve;  // Transparency (1.0 - dissolve)
		

		std::string diffuse_texname = mat.diffuse_texname;
		if (!diffuse_texname.empty()) {
			std::stringstream test(mat.diffuse_texname);
			std::string segment;
			std::vector<std::string> seglist;

			while (std::getline(test, segment, '/'))
			{
				seglist.push_back(segment);
			}

			std::cout << seglist[seglist.size() - 1] << std::endl;
			tmpMaterial.textureID = static_cast<uint32_t>(textureMap.at(seglist[seglist.size() - 1]));

		}
		


		
		this->materials.push_back(tmpMaterial);
	}


	std::unordered_map<Vertex, int, VertexHash> vertexMap;

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
			int material_id = shapes[s].mesh.material_ids[f];
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
				tmpVertex.materialID = static_cast<uint32_t>(material_id);
				
				if (vertices.capacity() <= vertices.size() + 1) {
					vertices.reserve(vertices.size() + 1000);
				}

				if (indices.capacity() <= indices.size() + 1) {
					indices.reserve(indices.size() + 1000);
				}


				auto const& it = vertexMap.find(tmpVertex);

				if (it ==  vertexMap.end()) {
					
					vertexMap.insert(std::pair<Vertex, int>(tmpVertex, vertices.size()));
					indices.push_back(static_cast<uint32_t>(vertices.size()));
					vertices.push_back(tmpVertex);

				}
				else {
					indices.push_back(static_cast<uint32_t>(it->second));
				}



				// Store the vertex index in the index buffer
			}
			index_offset += fv;

			// per-face material
		}

	}
	std::cout << this->name << " data size before shrinking vector" << std::endl;
	std::cout << "Vertices size: " << vertices.size() << " ** Capacity: " << vertices.capacity() << " Size in MB: " << float(vertices.size() * sizeof(Vertex)) / float(STORAGE_MB) << std::endl;
	std::cout << "Indices size: " << indices.size() << " ** Capacity: " << indices.capacity() << " Size in MB: " << float(indices.size() * sizeof(uint32_t)) / float(STORAGE_MB) << std::endl;
	
	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	std::cout << this->name << " data size after shrinking vector" << std::endl;
	std::cout << "Vertices size: " << vertices.size() << " ** Capacity: " << vertices.capacity() << " Size in MB: " << float(vertices.size() * sizeof(Vertex)) / float(STORAGE_MB) << std::endl;
	std::cout << "Indices size: " << indices.size() << " ** Capacity: " << indices.capacity() << " Size in MB: " << float(indices.size() * sizeof(uint32_t)) / float(STORAGE_MB) << std::endl;


}

uint32_t Object::getID()
{
	return objectID;
}