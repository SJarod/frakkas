#version 450 core

in vec3 vNormal;
in vec2 vUV;

out vec4 oColor;

uniform bool hasTexture = false;
uniform sampler2D tex;

void main()
{
    if (hasTexture)
        oColor = texture(tex, vUV);
    else
        oColor = vec4(1.0);
}