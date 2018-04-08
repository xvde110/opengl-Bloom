#version 330 core

layout (location = 0) out vec3 colorBuffer;
layout (location = 1) out vec3 hdrBuffer;


in vec3 Color;


void main()
{
	if(Color.x>1.0 || Color.y>1.0 || Color.z>1.0)
		hdrBuffer=Color;
	else
		hdrBuffer=vec3(0.0);
	colorBuffer=Color;
}