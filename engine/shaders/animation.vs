#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

layout(location = 3) in ivec4 aBoneIndices;
layout(location = 4) in vec4 aBoneWeights;

layout(std140, binding = 0) uniform uProjView
{
						//base alignment	//aligned offset
	mat4 projection;	//16				//0
						//16				//16
						//16				//32
						//16				//48
	mat4 view;			//16				//64
						//16				//80
						//16				//96
						//16				//112
};

uniform mat4 uModel;
uniform mat4 uModelNormal;

out vec3 vPos;
out vec3 vNormal;
out vec2 vUV;

const int MAX_BONES = 100;
uniform mat4 uBoneTransforms[MAX_BONES];

void main()
{
	mat4 boneMat = aBoneWeights[0] * uBoneTransforms[aBoneIndices[0]]
				 + aBoneWeights[1] * uBoneTransforms[aBoneIndices[1]]
				 + aBoneWeights[2] * uBoneTransforms[aBoneIndices[2]]
				 + aBoneWeights[3] * uBoneTransforms[aBoneIndices[3]];

	vec4 localPos = boneMat * vec4(aPos, 1.0);

	vec3 localNormal = mat3(uBoneTransforms[aBoneIndices[0]]) * aNormal;

	vNormal = (uModelNormal * vec4(localNormal, 0.0)).xyz;
	vUV = aUV;

	mat4 pvm = projection * view * uModel;
	gl_Position = pvm * vec4(localPos.xyz, 1.0);
}