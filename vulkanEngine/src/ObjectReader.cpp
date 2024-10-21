#include "ObjectReader.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"



ObjectReader::ObjectReader(ComponentList *list)
{
    this->list = list;
}

ObjectReader::~ObjectReader()
{
}

void ObjectReader::readDataOBJ(Entity e, const char *path)
{
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

	

	
    getComponent(list->comp_materials, list->map_materials, e)->reserve(100);

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
			tmpMaterial.textureName = seglist[seglist.size() - 1];

		}
		


		
		getComponent(list->comp_materials, list->map_materials, e)->push_back(tmpMaterial);
	}

    getComponent(list->comp_materials, list->map_materials, e)->shrink_to_fit();


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
				
				if ((getComponent(list->comp_vertices_3f, list->map_vertices_3f, e))->capacity() <= (getComponent(list->comp_vertices_3f, list->map_vertices_3f, e))->size() + 1) {
					(getComponent(list->comp_vertices_3f, list->map_vertices_3f, e))->reserve((getComponent(list->comp_vertices_3f, list->map_vertices_3f, e))->size() + 1000);
				}

				if (getComponent(list->comp_indices, list->map_indices, e)->capacity() <= getComponent(list->comp_indices, list->map_indices, e)->size() + 1) {
					getComponent(list->comp_indices, list->map_indices, e)->reserve(getComponent(list->comp_indices, list->map_indices, e)->size() + 1000);
				}


				auto const& it = vertexMap.find(tmpVertex);

				if (it ==  vertexMap.end()) {
					
					vertexMap.insert(std::pair<Vertex, int>(tmpVertex, (getComponent(list->comp_vertices_3f, list->map_vertices_3f, e))->size()));
					getComponent(list->comp_indices, list->map_indices, e)->push_back(static_cast<uint32_t>((getComponent(list->comp_vertices_3f, list->map_vertices_3f, e))->size()));
					(getComponent(list->comp_vertices_3f, list->map_vertices_3f, e))->push_back(tmpVertex);

				}
				else {
					getComponent(list->comp_indices, list->map_indices, e)->push_back(static_cast<uint32_t>(it->second));
				}



				// Store the vertex index in the index buffer
			}
			index_offset += fv;

			// per-face material
		}

	}
	(getComponent(list->comp_vertices_3f, list->map_vertices_3f, e))->shrink_to_fit();
	getComponent(list->comp_indices, list->map_indices, e)->shrink_to_fit();

	std::cout << getComponent(list->comp_name, list->map_name, e) << " data size " << std::endl;
	std::cout << "Vertices size: " << getComponent(list->comp_vertices_3f, list->map_vertices_3f, e)->size() << " ** Capacity: " << getComponent(list->comp_vertices_3f, list->map_vertices_3f, e)->capacity() << " Size in MB: " << float(getComponent(list->comp_vertices_3f, list->map_vertices_3f, e)->size() * sizeof(Vertex)) / float(STORAGE_MB) << std::endl;
	std::cout << "Indices size: " << getComponent(list->comp_indices, list->map_indices, e)->size() << " ** Capacity: " << getComponent(list->comp_indices, list->map_indices, e)->capacity() << " Size in MB: " << float(getComponent(list->comp_indices, list->map_indices, e)->size() * sizeof(uint32_t)) / float(STORAGE_MB) << std::endl;

}
