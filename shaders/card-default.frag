#version 400 core
out vec4 FragColor;

in vec2 textureCoordinates;

uniform sampler2D texture;

void main()
{
	FragColor = texture(texture1, textureCoordinates);
}	    
