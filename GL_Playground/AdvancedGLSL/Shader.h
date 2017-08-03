#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL\glew.h>

class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragPath, bool useLights = false);
	virtual ~Shader();

	void Use();

public:
	GLuint Program = 0;
	bool useLights;
};

#endif // !__SHADER_H__