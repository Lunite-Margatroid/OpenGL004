#version 330 core
in vec3 normal;
in vec3 fragPos;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 objColor;
uniform vec3 lightColor;
void main()
{
	float ambientStrength = 0.1;
	float lightStrength = 0.7;
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(vec3(lightPos) - fragPos);
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor * lightStrength;
	vec3 result = (ambientStrength + diffuse) * objColor;
	FragColor = vec4(result, 1.0);
}