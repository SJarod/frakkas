#version 450 core

struct Light
{
    bool enabled;
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct LightShadeResult
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;
};

uniform Material gDefaultMaterial = Material(
    vec3(0.8, 0.8, 0.8),
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, 1.0, 1.0),
    vec3(0.0, 0.0, 0.0),
    32.0);

in vec3 vPos;
in vec3 vNormal;
in vec2 vUV;

out vec4 oColor;

layout(std140, binding = 1) uniform uRendering
{
                        //base alignment    //aligned offset
    Light light;        //4                 //0
                        //16                //16
                        //16                //32
                        //16                //48
                        //16                //64
    bool toonShading;   //4                 //80
    bool outLine;       //4                 //84
    bool fiveTone;      //4                 //88
    vec3 cameraView;    //16                //96
};

uniform sampler2D uTexture;

LightShadeResult LightShade(Light light, float shininess, vec3 eyePosition, vec3 position, vec3 normal)
{
	LightShadeResult r = LightShadeResult(vec3(0.0), vec3(0.0), vec3(0.0));
	if (!light.enabled)
		return r;

    vec3 lightDir;
    if (light.position.w > 0.0) // Point light
    {
        vec3 lightPosFromVertexPos = (light.position.xyz / light.position.w) - position;
        lightDir = normalize(lightPosFromVertexPos);
        float dist = length(lightPosFromVertexPos);
    }
    else // Directional light
        lightDir = normalize(light.position.xyz);

    vec3 eyeDir  = normalize(eyePosition - position);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specAngle = max(dot(reflectDir, eyeDir), 0.0);

    r.ambient  = light.ambient;
    r.diffuse  = light.diffuse  * max(dot(normal, lightDir), 0.0);
    r.specular = light.specular * (pow(specAngle, shininess / 4.0));
	r.specular = clamp(r.specular, 0.0, 1.0);

	return r;
}

LightShadeResult GetLightsShading()
{
    LightShadeResult lightResult = LightShadeResult(vec3(0.0), vec3(0.0), vec3(0.0));

    LightShadeResult light = LightShade(light, gDefaultMaterial.shininess, cameraView, vPos, normalize(vNormal));
    lightResult.ambient  += light.ambient;
    lightResult.diffuse  += light.diffuse;
    lightResult.specular += light.specular;

    return lightResult;
}

void main()
{
    if (outLine)
    {
        oColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    float intensity = dot(normalize(light.position.xyz), normalize(vNormal));
    vec4 color1, color2;

    if (toonShading)
    {
        color1 = texture(uTexture, vUV);

        if (fiveTone)
        {
            if (intensity > 0.95)       color2 = vec4(1.0, 1.0, 1.0, 1.0);
            else if (intensity > 0.75)  color2 = vec4(0.8, 0.8, 0.8, 1.0);
            else if (intensity > 0.50)  color2 = vec4(0.6, 0.6, 0.6, 1.0);
            else if (intensity > 0.25)  color2 = vec4(0.4, 0.4, 0.4, 1.0);
            else                        color2 = vec4(0.2, 0.2, 0.2, 1.0);
        }
        else
        {
            if (intensity > 0.95)       color2 = vec4(1.0, 1.0, 1.0, 1.0);
            else if (intensity > 0.50)  color2 = vec4(0.6, 0.6, 0.6, 1.0);
            else if (intensity > 0.25)  color2 = vec4(0.4, 0.4, 0.4, 1.0);
            else                        color2 = vec4(0.2, 0.2, 0.2, 1.0);
        }

        oColor = color1 * color2;
    }
    else if (!toonShading)
    {
        // Compute phong shading
        LightShadeResult lightResult = GetLightsShading();

        vec3 ambientColor  = gDefaultMaterial.ambient * lightResult.ambient * texture(uTexture, vUV).rgb;
        vec3 diffuseColor  = gDefaultMaterial.diffuse * lightResult.diffuse * texture(uTexture, vUV).rgb;
        vec3 specularColor = gDefaultMaterial.specular * lightResult.specular;
        vec3 emissiveColor = gDefaultMaterial.emission * texture(uTexture, vUV).rgb;

        // Apply light color
        oColor = vec4((ambientColor + diffuseColor + specularColor + emissiveColor), 1.0);
    }
    else
        oColor = vec4(1.0);
}