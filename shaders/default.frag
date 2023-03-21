#version 400 core
out vec4 FragColor;

in vec2 textureCoordinates;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1, textureCoordinates), texture(texture2, vec2(1.0 - textureCoordinates.x, textureCoordinates.y)), 0.2);
}	    
