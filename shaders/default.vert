#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;

uniform mat4  modeMatrix;
uniform mat4  viewMatrix;
uniform mat4  projMatrix;

out vec3 textureColor;
out vec2 textureCoordinates;

void main()
{
	gl_Position        = projMatrix * viewMatrix * modeMatrix * vec4(aPos, 1.0);
	textureCoordinates = aTextCoord;
}