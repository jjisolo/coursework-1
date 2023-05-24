#include "AnimatedSprite.hpp"

void Engine::GFX::AnimatedSprite::move(vec2 spriteDestination) {
  auto spritePosition = getSpritePosition();
  m_TargetDestination = spriteDestination; 

  if(m_TargetDestination.x > spritePosition.x)
    m_MoveVector.x = -1.0f;
  else
    m_MoveVector.x = 1.0f;

  if(m_TargetDestination.y > spritePosition.y)
    m_MoveVector.y = -1.0f;
  else
    m_MoveVector.y = 1.0f;
}

void Engine::GFX::AnimatedSprite::animate(GLfloat elapsedTime) {
  auto spritePosition   = getSpritePosition();
  float spritePositionX = spritePosition.x;
  float spritePositionY = spritePosition.y;

  if(!APPROX(spritePosition.x, m_TargetDestination.x, 1.0f))
     spritePositionX = spritePosition.x + (m_MoveSpeed.x * elapsedTime)*m_MoveVector.x;
  if(!APPROX(spritePosition.y, m_TargetDestination.y, 1.0f))
     spritePositionY = spritePosition.y + (m_MoveSpeed.y * elapsedTime)*m_MoveVector.y;
     
  setSpritePosition({spritePositionX, spritePositionY}); 
}


