#version 450

struct objectProperties{
    uint objectID;
    uint materialID;
    uint textureID;
    uint normalMapID;
};


layout(binding = 0) uniform UniformBufferObject {
    mat4 model[1000];
    mat4 view;
    mat4 proj;
} ubo;



layout(binding = 3) uniform objectPropertiesBufferObject {
	objectProperties obj[1000];
} objectPropertiesBuffer;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in uint ID;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragPos;
layout(location = 4) flat out uint textureID;
layout(location = 5) flat out uint materialID;



void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model[ID] * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;

    fragPos = vec3(ubo.model[ID] * vec4(inPosition, 1.0));
    
    fragNormal = normalize(mat3(ubo.model[ID]) * inNormal);


    fragColor = inColor;

    textureID = objectPropertiesBuffer.obj[ID].textureID;
    materialID = objectPropertiesBuffer.obj[ID].materialID;

}