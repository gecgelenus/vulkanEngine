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
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorSetLayout();

	void updateUniformBuffer(uint32_t targetFrame, glm::vec3& position, glm::vec3& direction, glm::vec3& up);
	void setObjectTexture(std::string& name, Texture* texture);

	UniformBufferObjectVP ubo;
	void resetBuffers();

protected:
	std::vector<Vertex2D> vertices;
	std::vector<ObjectText*> objects;


};

