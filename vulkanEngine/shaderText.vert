#version 450



layout(binding = 0) uniform UniformBufferObject {
    mat4 model[1000];
    mat4 view;
    mat4 proj;
} ubo;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in uint ID;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) flat out uint textureID;
layout(location = 4) flat out uint materialID;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragNormal = inNormal;
    textureID = ID;
    materialID = ID;

}