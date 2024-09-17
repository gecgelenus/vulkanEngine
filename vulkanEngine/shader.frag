#version 450


layout(set = 0, binding = 1) uniform sampler texSampler;
layout(set = 0, binding = 2) uniform texture2D texImage[];


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;




layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(sampler2D(texImage[1], texSampler), fragTexCoord);
}