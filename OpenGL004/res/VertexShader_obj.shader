#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;

uniform mat4 modelTrans;
uniform mat4 viewTrans;
uniform mat4 projectionTrans;

void main()
{
	gl_Position = projectionTrans * viewTrans * modelTrans * vec4(aPos, 1.0);
	fragPos = vec3(modelTrans * vec4(aPos, 1.0));
	normal = aNormal;
}