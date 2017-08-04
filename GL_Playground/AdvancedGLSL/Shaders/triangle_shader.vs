#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const vec3 vertices[3] = vec3[3]( vec3(0.0f, 0.5f, 0.0f), vec3(-0.5f, 0.0f, 0.0f), vec3(0.5f, 0.0f, 0.0f) );

void main()
{
	//gl_Position = projection * view * model * vec4(vertices[gl_VertexID], 1.0);
	gl_Position = vec4(gl_VertexID, gl_VertexID, gl_VertexID, 1.0);
}