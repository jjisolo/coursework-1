// This file declares the `AnimatedSrite` class.
#pragma once

#include "_EngineIncludes.hpp"

#include "rendering/SpriteRenderer.hpp"
#include "rendering/TextureWrapper.hpp"

#include "Sprite.hpp"

using namespace std;
using namespace glm;

// This namespace is populated with all graphics-related stuff.
namespace Engine::GFX
{
  // This class is a wrapper around a static sprite class.
  //
  // This class purpose is to give the animation ability
  // to the sprites
  class AnimatedSprite: public Sprite
  {
  public:
    AnimatedSprite()
      : Sprite()
    {
    }

    void move(vec2 spriteDesination);
    
    void animate(GLfloat elapsedTime);

  protected:
    vec2 m_TargetDestination;
    vec2 m_MoveSpeed = {60.1f, 60.1f};
    vec2 m_MoveVector;
  };
}
