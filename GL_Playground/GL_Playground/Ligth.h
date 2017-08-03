#ifndef __LIGHT_H__
#define __LIGHT_H__

// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

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

	float constant;
	float linear;
	float quadratic;

	void SendInfoToShader(Shader* sh, int index)
	{

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

	}

};

#endif