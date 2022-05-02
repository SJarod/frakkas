#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

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

void main()
{
    vec4 pos4 = (uModel * vec4(aPos, 1.0));
    vPos = pos4.xyz / pos4.w;
    vNormal = (uModelNormal * vec4(aNormal, 0.0)).xyz;
	vUV = aUV;

	mat4 pvm = projection * view * uModel;
	gl_Position = pvm * vec4(aPos, 1.0);
}