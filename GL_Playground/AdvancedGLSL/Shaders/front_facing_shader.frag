#version 330 core

in vec3 ourNormal;
in vec2 ourUV;

out vec4 color;

uniform sampler2D checkers;
uniform sampler2D matrix;

void main()
{
	if(gl_FrontFacing)
		color = texture(checkers, ourUV);
	else
		color = texture(matrix, ourUV);
}