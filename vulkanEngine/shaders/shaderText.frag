#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout(set = 0, binding = 1) uniform sampler texSampler;
layout(set = 0, binding = 2) uniform texture2D texImage[];


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint textureID;


layout(location = 0) out vec4 outColor;

void main() {


	

	
	outColor = vec4(fragColor.r, fragColor.b, fragColor.g, texture(sampler2D(texImage[textureID], texSampler), fragTexCoord).a);
	


}