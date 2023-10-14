#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 modelTrans;
uniform mat4 viewTrans;
uniform mat4 projectionTrans;

void main()
{
	gl_Position = projectionTrans * viewTrans * modelTrans * vec4(aPos, 1.0);
}