#pragma once

#include "RenderBatch.hpp"
#include "ObjectText.hpp"


class RenderBatchText : public RenderBatch {
	
public:
	RenderBatchText();
	RenderBatchText(std::string& name, InstanceVariables& vars, const char* vertexPath, const char* fragmentPath);

	void addObject(ObjectText*);
	ObjectText* getObject(std::string& name);
	
	void createGraphicsPipeline();
	void createVertexBuffer(uint32_t bufferSize);
	void createIndexBuffer(uint32_t bufferSize);
	void createUniformBuffers();
	void createDescriptorSetLayout();
	void createDescriptorPool();
	void allocateDescriptorSets();

	void updateUniformBuffer(uint32_t targetFrame, glm::vec3& position, glm::vec3& direction, glm::vec3& up);
	void setObjectTexture(std::string& name, Texture* texture);

	UniformBufferObjectVP ubo;
	void resetBuffers();

protected:

	void updateGpuBuffers();
	uint32_t vertexBufferSize;
	uint32_t indexBufferSize;

	std::vector<Vertex2D> vertices;
	std::vector<ObjectText*> objects;


};

