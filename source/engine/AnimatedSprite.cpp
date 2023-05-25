#include "AnimatedSprite.hpp"

void Engine::GFX::AnimatedSprite::move(vec2 spriteDestination) {
  auto spritePosition = getSpritePosition();
  m_TargetDestination = spriteDestination; 
  m_IsAnimated        = true;

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

  const bool finishedX = APPROX(spritePosition.x, m_TargetDestination.x, 1.0f);
  const bool finishedY = APPROX(spritePosition.y, m_TargetDestination.y, 1.0f);

  const float deltaX = spritePositionX / m_TargetDestination.x;
  const float deltaY = spritePositionY / m_TargetDestination.y;

  float stepX = (m_MoveSpeed.x * elapsedTime) * m_MoveVector.x;
  float stepY = (m_MoveSpeed.y * elapsedTime) * m_MoveVector.y;

  stepX = deltaX * stepX;
  stepY = deltaY * stepY;
  
  if (!finishedX)
      spritePositionX = spritePosition.x + stepX;
  if (!finishedY)
      spritePositionY = spritePosition.y + stepY;
     
  if (finishedX && finishedY)
      m_IsAnimated = false;

  setSpritePosition({ spritePositionX, spritePositionY });
}


