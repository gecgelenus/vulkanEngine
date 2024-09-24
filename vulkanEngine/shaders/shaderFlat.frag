#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout(set = 0, binding = 1) uniform sampler texSampler;
layout(set = 0, binding = 2) uniform texture2D texImage[];

layout(set = 1, binding = 0) uniform LightBufferObject{
    vec4 lightPos[100];
    vec4 lightColor[100];
    vec4 ambientLight;
    vec4 cameraPos;
    uint count;
} lbo;


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragPos;
layout(location = 4) flat in uint textureID;
layout(location = 5) flat in uint materialID;






layout(location = 0) out vec4 outColor;

void main() {

    vec3 surfaceNormal = normalize(fragNormal);
    
    vec3 diffuseLight = lbo.ambientLight.xyz * lbo.ambientLight.a;

    vec3 specularLight = vec3(0.0f);

    vec3 viewDir = normalize(lbo.cameraPos.xyz - fragPos);


    for(int i = 0; i < lbo.count; i++){
        
	    vec3 lightDir = lbo.lightPos[i].xyz - fragPos;
    
        float attenuation = 1.0f / dot(lightDir, lightDir);
        // dot product of any vector with itself gives square of distance
        
        vec3 lightColor = attenuation * (lbo.lightColor[i].xyz * lbo.lightColor[i].a);
        
        lightDir = normalize(lightDir);

        vec3 diff = lightColor * max(dot(fragNormal, lightDir), 0);
        diffuseLight += diff;



        vec3 halfAngle = normalize(lightDir + viewDir);
        float specAngle = dot(surfaceNormal, halfAngle);
        specAngle = clamp(specAngle, 0, 1);
        float specular = pow(specAngle, 32.0);


        specularLight += specular * lightColor;

    }

    outColor =  vec4(fragColor * diffuseLight + specularLight * fragColor, 1.0);

}