#version 330 core

out vec4 color;

void main()
{
	if(gl_FragCoord.x < 1280 / 2)
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	else
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}