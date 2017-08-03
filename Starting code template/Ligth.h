#ifndef __LIGHT_H__
#define __LIGHT_H__

// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>

#include "Shader.h"

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

#endif