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
      : Sprite(), m_MoveSpeed({0.0f, 0.0f}), m_MoveVector({1.0f, 1.0f})
    {
    }

    // Getter and setter for the move speed class attribute.
    #define __gettersettertype glm::vec2
    makeGetterAndSetter(m_MoveSpeed, MoveSpeed);
    #undef  __gettersettertype

    // Actually set the move direction to the sprite.
    void move(vec2 spriteDesination);
    
    // Perfrom move step for the sprite
    void animate(GLfloat elapsedTime);

  protected:
    vec2 m_TargetDestination;
    vec2 m_MoveVector;
    vec2 m_MoveSpeed;
  };
}
