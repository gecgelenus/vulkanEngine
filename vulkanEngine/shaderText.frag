#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout(set = 0, binding = 1) uniform sampler texSampler;
layout(set = 0, binding = 2) uniform texture2D texImage[];


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) flat in uint textureID;
layout(location = 4) flat in uint materialID;





layout(location = 0) out vec4 outColor;

void main() {


	vec4 color = texture(sampler2D(texImage[textureID], texSampler), fragTexCoord);

	if(color.r < 0.5){
		outColor = vec4(0.0, 0.0, 0.0, 0.0);
	}else{
		outColor = texture(sampler2D(texImage[textureID], texSampler), fragTexCoord);
	}


}