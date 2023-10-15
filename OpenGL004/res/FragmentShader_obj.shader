#version 330 core
in vec3 vertexColor;

out vec4 FragColor;


void main()
{
	float ambientStrength = 0.1;
	vec3 result = ambientStrength * vertexColor;
	FragColor = vec4(result, 1.0);
}