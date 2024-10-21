#include "RenderBatch.hpp"
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#define VMA_IMPLEMENTATION
#include "vulkan_mem_alloc.h"

RenderBatch::RenderBatch()
{

	this->ubo = {};
}

RenderBatch::RenderBatch(std::string& name, InstanceVariables& vars, const char* vertexPath, const char* fragmentPath)
{
	constructorPriv(name.c_str(), vars, vertexPath, fragmentPath);
}

RenderBatch::RenderBatch(const char *name, InstanceVariables &vars, const char *vertexPath, const char *fragmentPath)
{
	constructorPriv(name, vars, vertexPath, fragmentPath);
}

RenderBatch::RenderBatch(ComponentList* list, const char *name, InstanceVariables &vars, const char *vertexPath, const char *fragmentPath)
{
	this->list = list;
	constructorPriv(name, vars, vertexPath, fragmentPath);
}

RenderBatch::RenderBatch(ComponentList* list, std::string& name, InstanceVariables &vars, const char *vertexPath, const char *fragmentPath)
{
	this->list = list;
	constructorPriv(name.c_str(), vars, vertexPath, fragmentPath);
}

RenderBatch::~RenderBatch()
{
}

void RenderBatch::addObject(Object* obj)
{
	uint32_t offset = this->vertices.size();
	uint32_t indexOffset = this->indices.size();


	obj->offset = offset;
	obj->indexOffset = indexOffset;

	obj->materialOffset = this->materialCount;
	obj->setID(objects.size());
	obj->setMaterialOffset(); // second update or more result undefined behaivor TODO: fix it
	obj->properties.objectID = objects.size();

	for (const Material& mtl : obj->materials) {
		mbo.ambient[materialCount] = glm::vec4(mtl.ambient, 1.0f);
		mbo.diffuse[materialCount] = glm::vec4(mtl.diffuse, mtl.transparency);
		mbo.specular[materialCount] = glm::vec4(mtl.specular, mtl.shininess);
		mbo.textureID[materialCount][0] = static_cast<uint32_t>(textureMap.at(mtl.textureName));
		materialCount++;
		if (MAX_MATERIAL_COUNT <= materialCount) {
			std::cout << "MATERIAL LIMIT EXCEEDED!!" << std::endl;
			break;
		}
	}



	this->vertices.insert(this->vertices.end(), obj->vertices.begin(), obj->vertices.end());
	this->indices.insert(this->indices.end(), obj->indices.begin(), obj->indices.end());

	

	this->objects.push_back(obj);

	VmaVirtualAllocationCreateInfo allocInfoVertex{};
	allocInfoVertex.size = obj->vertices.size() * sizeof(Vertex);

	VkResult res = vmaVirtualAllocate(virtualBlockVertex, &allocInfoVertex, &(obj->virtualVertexAllocation), &(obj->memoryOffsetVertex));

	if(res != VK_SUCCESS){
		throw std::runtime_error("Hmmm, it seems there is no room left for vertex data. Isn't there something you should handle ??");
	}

	VmaVirtualAllocationCreateInfo allocInfoIndex{};
	allocInfoIndex.size = obj->indices.size() * sizeof(uint32_t);

	res = vmaVirtualAllocate(virtualBlockIndex, &allocInfoIndex, &(obj->virtualIndexAllocation), &(obj->memoryOffsetIndex));

	if(res != VK_SUCCESS){
		throw std::runtime_error("Hmmm, it seems there is no room left for index data. Isn't there something you should handle ??");
	}

	obj->drawCommand.firstIndex = obj->memoryOffsetIndex / sizeof(uint32_t);
	obj->drawCommand.indexCount = obj->indices.size();
	obj->drawCommand.instanceCount = 1;
	obj->drawCommand.firstInstance = 0;
	obj->drawCommand.vertexOffset = obj->memoryOffsetVertex / sizeof(Vertex);

	drawCommands.push_back(obj->drawCommand);

	uploadObjectData(obj->getID());
	updateDrawBuffers();
	
	std::cout << "Object is added to render batch: " << obj->name << std::endl;
}

void RenderBatch::addObject(Entity e)
{
	uint32_t offset = this->vertices.size();
	uint32_t indexOffset = this->indices.size();

	setComponent(list->comp_mem_offset_vertex, list->map_mem_offset_vertex, e, offset);
	setComponent(list->comp_mem_offset_index, list->map_mem_offset_index, e, indexOffset);
	setComponent(list->comp_mem_offset_material, list->map_mem_offset_material, e, this->materialCount);
	setComponent(list->comp_model_id, list->map_model_id, e, static_cast<uint32_t>(objectsEntity.size()));
	updateID(e);
	updateMaterialOffset(e);
	getComponentP(list->comp_object_property, list->map_object_property, e)->objectID = objectsEntity.size();
	setColor(e);

	MaterialData matData = getComponent(list->comp_materials, list->map_materials, e);
	VertexData3F vertData = getComponent(list->comp_vertices_3f, list->map_vertices_3f, e);
	IndexData idxData = getComponent(list->comp_indices, list->map_indices, e);;


	for(int i = 0; i < matData->size(); i++, materialCount++){
		mbo.ambient[materialCount] = glm::vec4(matData->at(i).ambient, 1.0f);
		mbo.diffuse[materialCount] = glm::vec4(matData->at(i).diffuse, matData->at(i).transparency);
		mbo.specular[materialCount] = glm::vec4(matData->at(i).specular, matData->at(i).shininess);
		mbo.textureID[materialCount][0] = static_cast<uint32_t>(textureMap.at(matData->at(i).textureName));
		if (MAX_MATERIAL_COUNT <= materialCount) {
			std::cout << "MATERIAL LIMIT EXCEEDED!!" << std::endl;
			break;
		}
	}

	this->vertices.insert(this->vertices.end(), vertData->begin(), vertData->end());
	this->indices.insert(this->indices.end(), idxData->begin(), idxData->end());


	this->objectsEntity.push_back(e);
	
	VmaVirtualAllocationCreateInfo allocInfoVertex{};
	allocInfoVertex.size = vertData->size() * sizeof(Vertex);
	VirtualAllocation* virtAllocVertex = getComponentP(list->comp_virtual_alloc_vertex, list->map_virtual_alloc_vertex, e);
	VirtualAllocation* virtAllocIndex = getComponentP(list->comp_virtual_alloc_index, list->map_virtual_alloc_index, e);


	VkResult res = vmaVirtualAllocate(virtualBlockVertex, &allocInfoVertex, virtAllocVertex, getComponentP(list->comp_virtual_mem_offset_vertex,list->map_virtual_mem_offset_vertex, e));

	if(res != VK_SUCCESS){
		throw std::runtime_error("Hmmm, it seems there is no room left for vertex data. Isn't there something you should handle ??");
	}

	VmaVirtualAllocationCreateInfo allocInfoIndex{};
	allocInfoIndex.size = idxData->size() * sizeof(uint32_t);

	res = vmaVirtualAllocate(virtualBlockIndex, &allocInfoIndex, virtAllocIndex, getComponentP(list->comp_virtual_mem_offset_index, list->map_virtual_mem_offset_index, e));

	if(res != VK_SUCCESS){
		throw std::runtime_error("Hmmm, it seems there is no room left for index data. Isn't there something you should handle ??");
	}

	DrawCommand* drawCmd = getComponentP(list->comp_draw_command, list->map_draw_command, e);


	drawCmd->firstIndex = getComponent(list->comp_virtual_mem_offset_index, list->map_virtual_mem_offset_index, e) / sizeof(uint32_t);
	drawCmd->indexCount = idxData->size();
	drawCmd->instanceCount = 1;
	drawCmd->firstInstance = 0;
	drawCmd->vertexOffset = getComponent(list->comp_virtual_mem_offset_vertex, list->map_virtual_mem_offset_vertex, e) / sizeof(Vertex);


	drawCommands.push_back(*drawCmd);

	uploadEntityData(e);
	updateDrawBuffers();
	std::cout << "Object (as Entity) is added to render batch: " << getComponent(list->comp_name, list->map_name, e) << std::endl;
}

void RenderBatch::addLight(Light* light)
{
	light->ID = lights.size();

	lights.push_back(light);
}

void RenderBatch::deleteObject(std::string& name)
{
	deleteObject(name.c_str());
}

void RenderBatch::deleteObject(const char *name)
{
	Object* obj = getObject(name);

	objects.erase(objects.begin() + obj->getID());
	uint32_t objIndex = obj->drawCommand.firstIndex;
	
	for(int i = 0; i < drawCommands.size();i++){
		if(drawCommands[i].firstIndex = objIndex){
			drawCommands.erase(drawCommands.begin() + i);
			break;
		}
	}

	vmaVirtualFree(virtualBlockVertex, obj->virtualVertexAllocation);
	vmaVirtualFree(virtualBlockIndex, obj->virtualIndexAllocation);
}

void RenderBatch::deleteLight(uint32_t ID)
{
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i]->ID == ID) {
			lights.erase(lights.begin() + i);
			return;
		}
	}

	std::cout << "Light delete: light ID not found: " << ID << std::endl;
}

Object* RenderBatch::getObject(std::string& name)
{
	for (Object* obj : objects) {
		if (obj->name == name) {
			return obj;
		}
	}

	return nullptr;
}

Object* RenderBatch::getObject(const char* name)
{
	for (Object* obj : objects) {
		if (obj->name == name) {
			return obj;
		}
	}

	return nullptr;
}

void RenderBatch::setObjectTexture(std::string& name, Texture* texture)
{
	Object* obj = getObject(name);
	obj->properties.textureID = texture->textureID;
}

void RenderBatch::setAmbientLight(const glm::vec4& ambient)
{
	lbo.ambientLight = ambient;
}

void RenderBatch::readOBJ(std::string& path)
{


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

	std::vector<Object*> objectArr;

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		std::vector<Vertex> tmpVertices;
		std::vector<uint32_t> tmpIndices;
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
				tmpVertices.push_back(tmpVertex);

				// Store the vertex index in the index buffer
				tmpIndices.push_back(static_cast<uint32_t>(tmpIndices.size()));
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
		objectArr.push_back(new Object(shapes[s].name, tmpVertices, tmpIndices));
	}

	for (Object* obj : objectArr) {
		addObjectSingle(obj);
	}
	updateGpuBuffers();
}

void RenderBatch::readOBJ(const char* path)
{
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

	std::vector<Object*> objectArr;

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		std::vector<Vertex> tmpVertices;
		std::vector<uint32_t> tmpIndices;
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
				tmpVertex.color = glm::vec4(1.0f);
				tmpVertices.push_back(tmpVertex);

				// Store the vertex index in the index buffer
				tmpIndices.push_back(static_cast<uint32_t>(tmpIndices.size()));
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
		objectArr.push_back(new Object(shapes[s].name, tmpVertices, tmpIndices));
	}

	for (Object* obj : objectArr) {
		addObjectSingle(obj);
	}

	updateGpuBuffers();
}

void RenderBatch::createCommandPool()
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = 0;

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void RenderBatch::createCommandBuffers()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void RenderBatch::createVertexBuffer(uint32_t bufferSize)
{

	createBuffer((VkDeviceSize)bufferSize, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		vertexBuffer, vertexBufferAllocation);

}

void RenderBatch::createIndexBuffer(uint32_t bufferSize)
{

	createBuffer((VkDeviceSize)bufferSize, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		indexBuffer, indexBufferAllocation);
}

void RenderBatch::createDrawBuffer()
{
	createBuffer((VkDeviceSize)(sizeof(VkDrawIndexedIndirectCommand) * MEM_PREALLOCATE_COUNT_COMMAND), VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
		drawBuffer, drawBufferAllocation);
}

void RenderBatch::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersAllocation.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		

		VmaAllocationInfo info =  createBuffer(bufferSize, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
			VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, uniformBuffers[i], uniformBuffersAllocation[i]);

		uniformBuffersMapped[i] = info.pMappedData;
	}

	bufferSize = sizeof(objectProperties) * OBJECT_COUNT;

	objectPropertyBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	objectPropertyBuffersAllocation.resize(MAX_FRAMES_IN_FLIGHT);
	objectPropertyBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VmaAllocationInfo info = createBuffer(bufferSize, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
			VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, objectPropertyBuffers[i], objectPropertyBuffersAllocation[i]);

		objectPropertyBuffersMapped[i] = info.pMappedData;
	}

	bufferSize = sizeof(MaterialBufferObject);

	materialBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	materialBuffersAllocation.resize(MAX_FRAMES_IN_FLIGHT);
	materialBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VmaAllocationInfo info = createBuffer(bufferSize, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
			VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, materialBuffers[i], materialBuffersAllocation[i]);

		materialBuffersMapped[i] = info.pMappedData;
	}


	bufferSize = sizeof(LightBufferObject);

	lightBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	lightBuffersAllocation.resize(MAX_FRAMES_IN_FLIGHT);
	lightBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VmaAllocationInfo info = createBuffer(bufferSize, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
			VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, lightBuffers[i], lightBuffersAllocation[i]);

		lightBuffersMapped[i] = info.pMappedData;
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorBufferInfo lightBufferInfo{};
		lightBufferInfo.buffer = lightBuffers[i];
		lightBufferInfo.offset = 0;
		lightBufferInfo.range = sizeof(LightBufferObject);

		VkDescriptorBufferInfo propertybufferInfo{};
		propertybufferInfo.buffer = objectPropertyBuffers[i];
		propertybufferInfo.offset = 0;
		propertybufferInfo.range = sizeof(objectProperties) * OBJECT_COUNT;

		VkDescriptorBufferInfo materialBufferInfo{};
		materialBufferInfo.buffer = materialBuffers[i];
		materialBufferInfo.offset = 0;
		materialBufferInfo.range = sizeof(MaterialBufferObject);


		VkDescriptorImageInfo samplerInfo{};
		samplerInfo.sampler = textureSampler;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		VkWriteDescriptorSet descriptorWriteLight{};
		descriptorWriteLight.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteLight.dstSet = descriptorSetsLight[i];
		descriptorWriteLight.dstBinding = 0;
		descriptorWriteLight.dstArrayElement = 0;
		descriptorWriteLight.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWriteLight.descriptorCount = 1;
		descriptorWriteLight.pBufferInfo = &lightBufferInfo;
		descriptorWriteLight.pImageInfo = nullptr; // Optional
		descriptorWriteLight.pTexelBufferView = nullptr; // Optional

		VkWriteDescriptorSet descriptorWriteSampler{};
		descriptorWriteSampler.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteSampler.dstSet = descriptorSets[i];
		descriptorWriteSampler.dstBinding = 1;
		descriptorWriteSampler.dstArrayElement = 0;
		descriptorWriteSampler.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		descriptorWriteSampler.descriptorCount = 1;
		descriptorWriteSampler.pImageInfo = &samplerInfo;
		descriptorWriteSampler.pTexelBufferView = nullptr; // Optional

		VkWriteDescriptorSet descriptorWriteProperty{};
		descriptorWriteProperty.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteProperty.dstSet = descriptorSets[i];
		descriptorWriteProperty.dstBinding = 3;
		descriptorWriteProperty.dstArrayElement = 0;
		descriptorWriteProperty.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWriteProperty.descriptorCount = 1;
		descriptorWriteProperty.pBufferInfo = &propertybufferInfo;
		descriptorWriteProperty.pImageInfo = nullptr; // Optional
		descriptorWriteProperty.pTexelBufferView = nullptr; // Optional


		VkWriteDescriptorSet descriptorWriteMaterial{};
		descriptorWriteMaterial.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteMaterial.dstSet = descriptorSets[i];
		descriptorWriteMaterial.dstBinding = 4;
		descriptorWriteMaterial.dstArrayElement = 0;
		descriptorWriteMaterial.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWriteMaterial.descriptorCount = 1;
		descriptorWriteMaterial.pBufferInfo = &materialBufferInfo;
		descriptorWriteMaterial.pImageInfo = nullptr; // Optional
		descriptorWriteMaterial.pTexelBufferView = nullptr; // Optional

		VkWriteDescriptorSet sets[] = { descriptorWriteLight, descriptorWrite, descriptorWriteSampler, descriptorWriteProperty, descriptorWriteMaterial };

		vkUpdateDescriptorSets(device, 5, sets, 0, nullptr);



	}
}

void RenderBatch::addTexture(Texture* texture)
{
	textures.push_back(texture);
	texture->textureID = textureViews.size();
	textureViews.push_back(texture->getImageView());
	textureMap.insert(std::pair<std::string, int>(texture->path, texture->textureID));
	updateTextureDescriptors();
}



void RenderBatch::createDescriptorPool()
{
	VkDescriptorPoolSize uniformPool{};
	uniformPool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uniformPool.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolSize lightPool{};
	lightPool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	lightPool.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolSize materialPool{};
	materialPool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	materialPool.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);


	VkDescriptorPoolSize texturePool{};
	texturePool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	texturePool.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolSize samplerPool{};
	samplerPool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	samplerPool.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolSize objectPropertyPool{};
	objectPropertyPool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	objectPropertyPool.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolSize sizes[] = { uniformPool, texturePool, samplerPool, objectPropertyPool, lightPool, materialPool };

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 6;
	poolInfo.pPoolSizes = sizes;
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT*2);

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void RenderBatch::createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding textureBinding{};
	textureBinding.binding = 2;
	textureBinding.descriptorCount = 64;
	textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	textureBinding.pImmutableSamplers = nullptr;
	textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding objectPropertyBinding{};
	objectPropertyBinding.binding = 3;
	objectPropertyBinding.descriptorCount = 1;
	objectPropertyBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	objectPropertyBinding.pImmutableSamplers = nullptr;
	objectPropertyBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding materialBufferBinding{};
	materialBufferBinding.binding = 4;
	materialBufferBinding.descriptorCount = 1;
	materialBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	materialBufferBinding.pImmutableSamplers = nullptr;
	materialBufferBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding bindings[] = { uboLayoutBinding , samplerLayoutBinding, textureBinding, objectPropertyBinding, materialBufferBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 5;
	layoutInfo.pBindings = bindings;


	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}


	VkDescriptorSetLayoutBinding lightBinding{};
	lightBinding.binding = 0;
	lightBinding.descriptorCount = 1;
	lightBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	lightBinding.pImmutableSamplers = nullptr;
	lightBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding bindingsLight[] = {lightBinding };


	VkDescriptorSetLayoutCreateInfo layoutInfoLight{};
	layoutInfoLight.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfoLight.bindingCount = 1;
	layoutInfoLight.pBindings = bindingsLight;

	if (vkCreateDescriptorSetLayout(device, &layoutInfoLight, nullptr, &descriptorSetLayoutLight) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}

}

void RenderBatch::allocateDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);


	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}


	std::vector<VkDescriptorSetLayout> layoutsLight(MAX_FRAMES_IN_FLIGHT, descriptorSetLayoutLight);

	VkDescriptorSetAllocateInfo allocInfoLight{};
	allocInfoLight.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfoLight.descriptorPool = descriptorPool;
	allocInfoLight.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfoLight.pSetLayouts = layoutsLight.data();

	descriptorSetsLight.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(device, &allocInfoLight, descriptorSetsLight.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

void RenderBatch::createTextureSampler()
{
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(pDevice, &properties);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;

	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void RenderBatch::updateUniformBuffer(uint32_t targetFrame, glm::vec3& position, glm::vec3& direction, glm::vec3& up, float FOV, float nearPlane, float farPlane)
{

	std::vector<objectProperties> propertyArray(1000);
	lbo.cameraPos = glm::vec4(position, 1.0f);


	for (Object* obj : objects) {
		propertyArray[obj->getID()] = obj->properties;
	}

	for (Object* obj : objects) {
		ubo.model[propertyArray[obj->getID()].objectID] = obj->modelMatrix;
	}

	ubo.view = glm::lookAt(position, position + direction, up);

	ubo.proj = glm::perspective(glm::radians(FOV), WIDTH / (float)HEIGHT, nearPlane, farPlane);

	ubo.proj[1][1] *= -1;

	memcpy(uniformBuffersMapped[targetFrame], &ubo, sizeof(ubo));

	memcpy(objectPropertyBuffersMapped[targetFrame], propertyArray.data(), sizeof(objectProperties) * OBJECT_COUNT);

	memcpy(materialBuffersMapped[targetFrame], &mbo, sizeof(MaterialBufferObject));

}

void RenderBatch::updateUniformBufferEntity(uint32_t targetFrame, glm::vec3 &position, glm::vec3 &direction, glm::vec3 &up, float FOV, float nearPlane, float farPlane)
{

	std::vector<objectProperties> propertyArray(1000);
	lbo.cameraPos = glm::vec4(position, 1.0f);

	for (Entity e: objectsEntity) {
		propertyArray[getComponent(list->comp_model_id, list->map_model_id, e)] = getComponent(list->comp_object_property, list->map_object_property, e);
	}

	for (Entity e: objectsEntity) {
		ubo.model[propertyArray[getComponent(list->comp_model_id, list->map_model_id, e)].objectID] = getComponent(list->comp_model_matrix, list->map_model_matrix, e);
	}


	ubo.view = glm::lookAt(position, position + direction, up);

	ubo.proj = glm::perspective(glm::radians(FOV), WIDTH / (float)HEIGHT, nearPlane, farPlane);

	ubo.proj[1][1] *= -1;
	ubo.model[1] = glm::mat4(5.0f);
	memcpy(uniformBuffersMapped[targetFrame], &ubo, sizeof(ubo));
	memcpy(objectPropertyBuffersMapped[targetFrame], propertyArray.data(), sizeof(objectProperties) * OBJECT_COUNT);

	memcpy(materialBuffersMapped[targetFrame], &mbo, sizeof(MaterialBufferObject));
}



void RenderBatch::updateLightBuffer(uint32_t targetFrame)
{
	lbo.lightCount = lights.size();

	for (int i = 0; i < lbo.lightCount; i++) {
		lbo.lightPosArr[i] = lights[i]->position;
		lbo.lightColorArr[i] = lights[i]->color;
	}


	memcpy(lightBuffersMapped[targetFrame], &lbo, sizeof(lbo));

}

void RenderBatch::createGraphicsPipeline()
{
	//SHADER CREATION

	auto vertShaderCode = readFile(vertexPath);
	auto fragShaderCode = readFile(fragmentPath);

	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// DYNAMIC STATES

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	// VERTEX INPUT

	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription attributeDescription0{};
	attributeDescription0.binding = 0;
	attributeDescription0.location = 0;
	attributeDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescription0.offset = offsetof(Vertex, pos);

	VkVertexInputAttributeDescription attributeDescription1{};
	attributeDescription1.binding = 0;
	attributeDescription1.location = 1;
	attributeDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescription1.offset = offsetof(Vertex, normal);

	VkVertexInputAttributeDescription attributeDescription2{};
	attributeDescription2.binding = 0;
	attributeDescription2.location = 2;
	attributeDescription2.format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescription2.offset = offsetof(Vertex, color);

	VkVertexInputAttributeDescription attributeDescription3{};
	attributeDescription3.binding = 0;
	attributeDescription3.location = 3;
	attributeDescription3.format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescription3.offset = offsetof(Vertex, texCoord);

	VkVertexInputAttributeDescription attributeDescription4{};
	attributeDescription4.binding = 0;
	attributeDescription4.location = 4;
	attributeDescription4.format = VK_FORMAT_R32_UINT;
	attributeDescription4.offset = offsetof(Vertex, ID);

	VkVertexInputAttributeDescription attributeDescription5{};
	attributeDescription5.binding = 0;
	attributeDescription5.location = 5;
	attributeDescription5.format = VK_FORMAT_R32_UINT;
	attributeDescription5.offset = offsetof(Vertex, materialID);

	VkVertexInputAttributeDescription descriptions[] = { attributeDescription0 , attributeDescription1, attributeDescription2, attributeDescription3, attributeDescription4, attributeDescription5 };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 6;
	vertexInputInfo.pVertexAttributeDescriptions = descriptions; // Optional

	// INPUT ASSEMBLY

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// VIEWPORT AND SCISSOR

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// RASTERIZER

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	// DEPTH AND STENCIL

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	// MULTISAMPLING
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_TRUE;
	multisampling.minSampleShading = .2f;
	multisampling.rasterizationSamples = msaaSamples;
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	// COLOR BLENDING

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	// PIPELINE LAYOUT

	VkDescriptorSetLayout layouts[] = { descriptorSetLayout , descriptorSetLayoutLight};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 2;
	pipelineLayoutInfo.pSetLayouts = layouts;
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	// CLEANUP

	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void RenderBatch::createVirtualBlock()
{
	VmaVirtualBlockCreateInfo vertexInfo = {};
	vertexInfo.size = MEM_PREALLOCATE_SIZE_VERTEX;
 
	VkResult res = vmaCreateVirtualBlock(&vertexInfo, &virtualBlockVertex);

	VmaVirtualBlockCreateInfo indexInfo = {};
	indexInfo.size = MEM_PREALLOCATE_SIZE_INDEX;
 
	res = vmaCreateVirtualBlock(&indexInfo, &virtualBlockIndex);
}

void RenderBatch::uploadObjectData(uint32_t index)
{

	VkDeviceSize vertexSize = objects[index]->vertices.size() * sizeof(Vertex);
	VkDeviceSize indexSize = objects[index]->indices.size() * sizeof(uint32_t);

	// VERTEX DATA
	VkBuffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;

	createBuffer(vertexSize, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
		VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		stagingBuffer, stagingBufferAllocation);

	void* data;
	vmaMapMemory(allocator, stagingBufferAllocation, &data);
	memcpy(data, objects[index]->vertices.data(), (size_t)vertexSize);
	vmaUnmapMemory(allocator, stagingBufferAllocation);

	copyBuffer(stagingBuffer, vertexBuffer, vertexSize, 0, objects[index]->memoryOffsetVertex);

	vmaFreeMemory(allocator, stagingBufferAllocation);


	// INDEX DATA
	
	createBuffer(indexSize, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
		VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		stagingBuffer, stagingBufferAllocation);

	void* data2;
	vmaMapMemory(allocator, stagingBufferAllocation, &data2);
	memcpy(data2, objects[index]->indices.data(), (size_t)indexSize);
	vmaUnmapMemory(allocator, stagingBufferAllocation);


	copyBuffer(stagingBuffer, indexBuffer, indexSize, 0, objects[index]->memoryOffsetIndex);

	vmaFreeMemory(allocator, stagingBufferAllocation);


}

void RenderBatch::uploadEntityData(Entity e)
{
	VertexData3F vertData = getComponent(list->comp_vertices_3f, list->map_vertices_3f, e);
	IndexData idxData = getComponent(list->comp_indices, list->map_indices, e);;

	VkDeviceSize vertexSize = vertData->size() * sizeof(Vertex);
	VkDeviceSize indexSize = idxData->size() * sizeof(uint32_t);

	// VERTEX DATA
	VkBuffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;

	createBuffer(vertexSize, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
		VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		stagingBuffer, stagingBufferAllocation);

	void* data;
	vmaMapMemory(allocator, stagingBufferAllocation, &data);
	memcpy(data, vertData->data(), (size_t)vertexSize);
	vmaUnmapMemory(allocator, stagingBufferAllocation);

	copyBuffer(stagingBuffer, vertexBuffer, vertexSize, 0, getComponent(list->comp_virtual_mem_offset_vertex, list->map_virtual_mem_offset_vertex, e));

	vmaFreeMemory(allocator, stagingBufferAllocation);


	// INDEX DATA
	
	createBuffer(indexSize, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
		VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		stagingBuffer, stagingBufferAllocation);

	void* data2;
	vmaMapMemory(allocator, stagingBufferAllocation, &data2);
	memcpy(data2, idxData->data(), (size_t)indexSize);
	vmaUnmapMemory(allocator, stagingBufferAllocation);


	copyBuffer(stagingBuffer, indexBuffer, indexSize, 0, getComponent(list->comp_virtual_mem_offset_index, list->map_virtual_mem_offset_index, e));

	vmaFreeMemory(allocator, stagingBufferAllocation);
}

VmaAllocationInfo RenderBatch::createBuffer(VkDeviceSize size, int memoryType, VkBufferUsageFlags usage, VkBuffer& buffer, VmaAllocation& allocation)
{
	VmaAllocationInfo info;

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VmaAllocationCreateInfo allocInfo{};
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
	allocInfo.flags = memoryType;
	vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &buffer, &allocation, &info);

	return info;
}

uint32_t RenderBatch::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(pDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void RenderBatch::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void RenderBatch::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = srcOffset; // Optional
	copyRegion.dstOffset = dstOffset; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void RenderBatch::resetBuffers()
{
	vertices.clear();
	indices.clear();

	for (int i = 0; i < objects.size(); i++) {
		uint32_t offset = this->vertices.size();
		uint32_t indexOffset = this->indices.size();
		
		objects[i]->offset = offset;
		objects[i]->indexOffset = indexOffset;
		objects[i]->setID(i);
		objects[i]->properties.objectID = i;
		std::cout << "ID: " << objects[i]->getID() << std::endl;

		this->vertices.insert(this->vertices.end(), objects[i]->vertices.begin(), objects[i]->vertices.end());
		this->indices.insert(this->indices.end(), objects[i]->indices.begin(), objects[i]->indices.end());

		
	}

	updateGpuBuffers();
}

void RenderBatch::constructorPriv(const char *name, InstanceVariables &vars, const char *vertexPath, const char *fragmentPath)
{

	this->name = name;
	this->renderFlag = true;
	this->vertexBufferSize = 0;
	this->indexBufferSize = 0;

    this->instance = vars.instance;
    this->device = vars.device;
    this->pDevice = vars.physicalDevice;
    this->graphicsQueue = vars.graphicsQueue;
    this->presentQueue = vars.presentQueue;
    this->window = vars.window;
    this->swapChain = vars.swapchain;
    this->swapChainFramebuffers = vars.swapchainFramebuffers;
    this->swapChainExtent = vars.swapchainExtent;
    this->surface = vars.surface;
    this->renderPass = vars.renderpass;
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;
	this->allocator = vars.allocator;

	this->materialCount = 0;

	createCommandPool();
	createCommandBuffers();
	createDescriptorPool();
	createDescriptorSetLayout();
	allocateDescriptorSets();
	createTextureSampler();
	createGraphicsPipeline();
	createUniformBuffers();
	createVertexBuffer(MEM_PREALLOCATE_SIZE_VERTEX);
	createIndexBuffer(MEM_PREALLOCATE_SIZE_INDEX);
	createVirtualBlock();
	createDrawBuffer();
}

void RenderBatch::addObjectSingle(Object *obj)
{
	uint32_t offset = this->vertices.size();

	obj->offset = offset;
	obj->materialOffset = this->materialCount;
	obj->setID(objects.size());
	obj->setMaterialOffset(); // second update or more result undefined behaivor TODO: fix it
	obj->properties.objectID = objects.size();
	std::cout << "ID: " << obj->getID() << std::endl;

	for (const Material& mtl : obj->materials) {
		mbo.ambient[materialCount] = glm::vec4(mtl.ambient, 1.0f);
		mbo.diffuse[materialCount] = glm::vec4(mtl.diffuse, mtl.transparency);
		mbo.specular[materialCount] = glm::vec4(mtl.specular, mtl.shininess);
		mbo.textureID[materialCount][0] = mtl.textureID;
		materialCount++;
		if (MAX_MATERIAL_COUNT <= materialCount) {
			std::cout << "MATERIAL LIMIT EXCEEDED!!" << std::endl;
			break;
		}
	}



	this->vertices.insert(this->vertices.end(), obj->vertices.begin(), obj->vertices.end());

	for (uint32_t i : obj->indices) {
		this->indices.push_back(i + offset);
	}

	this->objects.push_back(obj);




	std::cout << "Object is added to render batch: " << obj->name << std::endl;
}

void RenderBatch::updateGpuBuffers()
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	if (vertexBufferSize != bufferSize) {
		if (vertexBufferSize != 0) {
			vmaDestroyBuffer(allocator, vertexBuffer, vertexBufferAllocation);
		}
		vertexBufferSize = bufferSize;

		createVertexBuffer(vertexBufferSize);
	}


	// VERTEX

	VkBuffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;

	createBuffer(bufferSize, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
		VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		stagingBuffer, stagingBufferAllocation);

	void* data;
	vmaMapMemory(allocator, stagingBufferAllocation, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vmaUnmapMemory(allocator, stagingBufferAllocation);


	copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vmaFreeMemory(allocator, stagingBufferAllocation);

	// INDEX

	bufferSize = sizeof(indices[0]) * indices.size();

	if (indexBufferSize != bufferSize) {

		if (indexBufferSize != 0) {
			vmaDestroyBuffer(allocator, indexBuffer, indexBufferAllocation);
		}
		indexBufferSize = bufferSize;

		createIndexBuffer(indexBufferSize);
	}

	createBuffer(bufferSize, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
		VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		stagingBuffer, stagingBufferAllocation);

	void* data2;

	vmaMapMemory(allocator, stagingBufferAllocation, &data2);
	memcpy(data2, indices.data(), (size_t)bufferSize);
	vmaUnmapMemory(allocator, stagingBufferAllocation);

	copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vmaFreeMemory(allocator, stagingBufferAllocation);

}

void RenderBatch::updateDrawBuffers()
{
	VkBuffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;

	createBuffer((VkDeviceSize)(sizeof(VkDrawIndexedIndirectCommand) * drawCommands.size()), VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
		VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		stagingBuffer, stagingBufferAllocation);

	void* data;
	vmaMapMemory(allocator, stagingBufferAllocation, &data);
	memcpy(data, drawCommands.data(), (size_t)(sizeof(VkDrawIndexedIndirectCommand) * drawCommands.size()));
	vmaUnmapMemory(allocator, stagingBufferAllocation);


	copyBuffer(stagingBuffer, drawBuffer, (VkDeviceSize)(sizeof(VkDrawIndexedIndirectCommand) *  drawCommands.size()));

	vmaFreeMemory(allocator, stagingBufferAllocation);
}

VkShaderModule RenderBatch::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}

std::vector<char> RenderBatch::readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::cout << "Reading file: " << filename << " size: " << fileSize << std::endl;
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

void RenderBatch::updateTextureDescriptors()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		std::vector<VkDescriptorImageInfo> descriptorInfos;

		for (int i = 0; i < textureViews.size(); i++) {
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = textureViews[i];

			descriptorInfos.push_back(imageInfo);
		}

		VkWriteDescriptorSet descriptorWriteTex{};
		descriptorWriteTex.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteTex.dstSet = descriptorSets[i];
		descriptorWriteTex.dstBinding = 2;
		descriptorWriteTex.dstArrayElement = 0;
		descriptorWriteTex.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		descriptorWriteTex.descriptorCount = descriptorInfos.size();
		descriptorWriteTex.pImageInfo = descriptorInfos.data();
		descriptorWriteTex.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device, 1, &descriptorWriteTex, 0, nullptr);
	}
}




void RenderBatch::deleteTexture(Texture* texture)
{
	//TODO
}

void RenderBatch::updateMaterialOffset(Entity e)
{
	MemoryOffset matOffset = getComponent(list->comp_mem_offset_material, list->map_mem_offset_material, e);
	VertexData3F objVertices = getComponent(list->comp_vertices_3f, list->map_vertices_3f, e);

	for (int i = 0; i < objVertices->size(); i++) {
		(*objVertices)[i].materialID += matOffset;
	}
}

void RenderBatch::updateID(Entity e)
{
	ID modelID = getComponent(list->comp_model_id, list->map_model_id, e);
	VertexData3F objVertices = getComponent(list->comp_vertices_3f, list->map_vertices_3f, e);

	for (int i = 0; i < objVertices->size(); i++) {
		(*objVertices)[i].ID = modelID;
	}
}

void RenderBatch::setColor(Entity e)
{
	VertexData3F objVertices = getComponent(list->comp_vertices_3f, list->map_vertices_3f, e);

	for (int i = 0; i < objVertices->size(); i++) {
		(*objVertices)[i].color += glm::vec4(1.0f);
	}
}
