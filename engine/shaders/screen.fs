out vec4 oColor;

in vec2 vUV;

uniform sampler2D uRenderTex;

void main()
{
	oColor = texture(uRenderTex, vUV);
}