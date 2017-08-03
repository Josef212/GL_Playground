#ifndef __LIGHT_H__
#define __LIGHT_H__

// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>

#include "Shader.h"

struct PointLight
{
	PointLight() {}
	PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec) : position(pos), ambient(amb), diffuse(dif), specular(spec)
	{}

	~PointLight() {}

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	glm::vec3 position;

	float constant = 1.f;
	float linear = 0.09f;
	float quadratic = 0.032;

	bool enabled = true;

	void SendInfoToShader(Shader* sh, int index)
	{
		std::string num = std::to_string(index);
		glUniform1i(glGetUniformLocation(sh->Program, ("pointLights[" + num + " ].enabled").c_str()), (enabled == true ? 1 : 0));
		
		glUniform3f(glGetUniformLocation(sh->Program, ("pointLights[" + num + " ].position").c_str()), position.x, position.y, position.z);
		glUniform3f(glGetUniformLocation(sh->Program, ("pointLights[" + num + "].ambient").c_str()), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(sh->Program, ("pointLights[" + num + "].diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(sh->Program, ("pointLights[" + num + "].specular").c_str()), specular.x, specular.y, specular.z);
		glUniform1f(glGetUniformLocation(sh->Program, ("pointLights[" + num + "].constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(sh->Program, ("pointLights[" + num + "].linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(sh->Program, ("pointLights[" + num + "].quadratic").c_str()), quadratic);
		
	}
	
};

struct DirectionalLight
{
	DirectionalLight() {}
	DirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec) : direction(dir), ambient(amb), diffuse(dif), specular(spec)
	{}

	~DirectionalLight() {}

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	glm::vec3 direction;

	void SendInfoToShader(Shader* sh)
	{
		glUniform3f(glGetUniformLocation(sh->Program, "dirLight.direction"), direction.x, direction.y, direction.z);

		glUniform3f(glGetUniformLocation(sh->Program, "dirLight.ambient"), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(sh->Program, "dirLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(sh->Program, "dirLight.specular"), specular.x, specular.y, specular.z);
	}

};

extern DirectionalLight* dirLight;
extern std::vector<PointLight*> pointLights;

#endif