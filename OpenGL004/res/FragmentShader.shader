#version 330 core
out vec4 FragColor;
uniform vec4 theColor;
void main()
{
	FragColor = theColor;
}