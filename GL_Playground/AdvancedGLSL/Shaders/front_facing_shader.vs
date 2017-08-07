#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourNormal;
out vec2 ourUV;

void main()
{
	ourNormal = normal;
	ourUV = uv;
	gl_Position = projection * view * model * vec4(position, 1.0);
}