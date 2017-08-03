#version 330 core

#define MAX_POINT_LIGHTS 4

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int enabled;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform float shininess;
uniform float point_lights_count;

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    //First calc directional light
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    //Then point lights
    for(int i = 0; i < MAX_POINT_LIGHTS; ++i)
    	if(pointLights[i].enabled == 1)
    		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

    //Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    //Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    //Combine results
    vec3 ambient    = light.ambient     * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse    = light.diffuse     * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular   = light.specular    * spec * vec3(texture(texture_specular1, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    //Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    //Attenuation
    float distance      = length(light.position - fragPos);
    float attenuation   = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //Combine results
    vec3 ambient    = light.ambient     * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse    = light.diffuse     * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular   = light.specular    * spec * vec3(texture(texture_specular1, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}