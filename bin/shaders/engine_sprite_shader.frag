#version 330 core

in  vec2 textureCoordinates;
out vec4 color;

uniform vec2 textureResolution;
uniform vec2 screenResolution;

uniform sampler2D image;
uniform vec3 spriteColor;

// Effects accessors
uniform int applyGlowingEffect = 0;
uniform int applyMotionEffect  = 0;

uniform float elapsedTime        = 0.0;
uniform vec3  intencityMask      = vec3(0.6, 0.8, 0.8);

void main() 
{
    vec4  _spriteColor   = vec4(spriteColor, 1.0); 
    float intencity      = sin(elapsedTime);

    if(applyGlowingEffect == 1)
    {
      // Glowing effect whenever user hovers the card on the sprite
      if(applyGlowingEffect == 1) 
      {
        for(int i = 0; i < 3; ++i) 
        {
          _spriteColor[i] = (intencityMask[i]) + (intencity*(1-intencityMask[i])); 
        }
      } 

      color = _spriteColor * texture(image, textureCoordinates);
    }
    
    color = _spriteColor * texture(image, textureCoordinates);

    if(applyMotionEffect == 1)
    {
      color.w = 0.1;
    }
}