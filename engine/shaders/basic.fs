#version 450 core

struct Light
{
    bool enabled;
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct light_shade_result
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;
};

uniform material gDefaultMaterial = material(
    vec3(0.0, 0.0, 0.0),
    vec3(0.8, 0.8, 0.8),
    vec3(1.0, 1.0, 1.0),
    vec3(0.0, 0.0, 0.0),
    32.0);

in vec3 vPos;
in vec3 vNormal;
in vec2 vUV;

out vec4 oColor;

uniform Light uLight;
uniform bool uToonShading;
uniform bool uOutline;
uniform bool uFiveTone;
uniform bool hasTexture = false;
uniform sampler2D uTexture;
uniform vec3 uCameraView;

light_shade_result light_shade(Light light, float shininess, vec3 eyePosition, vec3 position, vec3 normal)
{
	light_shade_result r = light_shade_result(vec3(0.0), vec3(0.0), vec3(0.0));
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

light_shade_result get_lights_shading()
{
    light_shade_result lightResult = light_shade_result(vec3(0.0), vec3(0.0), vec3(0.0));

    light_shade_result light = light_shade(uLight, gDefaultMaterial.shininess, uCameraView, vPos, normalize(vNormal));
    lightResult.ambient  += light.ambient;
    lightResult.diffuse  += light.diffuse;
    lightResult.specular += light.specular;

    return lightResult;
}

void main()
{
    float intensity = dot(normalize(uLight.position.xyz), normalize(vNormal));
    vec4 color1, color2;

    if (uToonShading)
    {
        if (hasTexture)
            color1 = texture(uTexture, vUV);
        else
            color1 = vec4(uLight.diffuse, 1.0);

        if (uFiveTone)
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
    else if (!uToonShading && hasTexture)
    {
        // Compute phong shading
        light_shade_result lightResult = get_lights_shading();

        vec3 ambientColor  = gDefaultMaterial.ambient * lightResult.ambient;
        vec3 diffuseColor  = gDefaultMaterial.diffuse * lightResult.diffuse * texture(uTexture, vUV).rgb;
        vec3 specularColor = gDefaultMaterial.specular * lightResult.specular;
        vec3 emissiveColor = gDefaultMaterial.emission + texture(uTexture, vUV).rgb;

        // Apply light color
        oColor = vec4((ambientColor + diffuseColor + specularColor + emissiveColor), 1.0);
    }
    else
        oColor = vec4(1.0);

    if (uOutline)
        oColor = vec4(0.0, 0.0, 0.0, 1.0);
}
