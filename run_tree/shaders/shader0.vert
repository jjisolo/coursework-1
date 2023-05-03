#version 330 core
layout (location = 0) in vec4 vertexIn;

out vec2 TextureCoordinates;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main()
{
    TextureCoordinates = vertexIn.zw;
    gl_Position = projectionMatrix* modelMatrix * vec4(vertexIn.zy, 0.0, 1.0);
}