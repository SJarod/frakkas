#version 450 core

struct Light
{
    bool enabled;
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light uLight;

in vec3 vPos;
in vec3 vNormal;
in vec2 vUV;

out vec4 oColor;

uniform bool hasTexture = false;
uniform sampler2D uTexture;
uniform vec3 uCameraView;

void main()
{
    if (uLight.enabled && hasTexture)
    {
        vec3 lightDir = normalize(uLight.position.xyz);
        vec3 viewDir = normalize(uCameraView - vPos);

        vec3 ambientColor = uLight.ambient;

        float lambertian = max(dot(lightDir, normalize(vNormal)), 0.0);
        vec3 diffuseColor = uLight.diffuse * lambertian;

        vec3 reflectDir = reflect(-lightDir, normalize(vNormal));
        float spec = pow(max(dot(reflectDir, viewDir), 0.0), 15);
        vec3 specularColor = uLight.specular * spec;

        oColor = vec4((ambientColor + diffuseColor + specularColor), 1.0) * texture(uTexture, vUV);
    }
    else if (!uLight.enabled && hasTexture)
        oColor = texture(uTexture, vUV);
    else
        oColor = vec4(1.0);
}