#pragma once

#include "string"
#include "utility/GetSetMacro.hpp"
#include "glm/glm.hpp"

namespace Engine::GFX
{
	class Sprite
	{
	public:
		explicit Sprite() :
			m_SpritePosition   ({ 0.0f, 0.0f }),       m_SpriteSize({ 0.0f, 0.0f }),
			m_SpriteColor      ({ 0.0f, 0.0f, 0.0f }), m_SpriteRotation(0.0f),
			m_SpriteTexturePath("undefined")
		{
		}

		#define __gettersettertype glm::vec2
		makeGetterAndSetter(m_SpritePosition, SpritePosition);
		makeGetterAndSetter(m_SpriteSize,     SpriteSize);

		#define __gettersettertype glm::vec3
		makeGetterAndSetter(m_SpriteColor, SpriteColor);

		#define __gettersettertype std::string
		makeGetterAndSetter(m_SpriteTexturePath, SpriteTexturePath);
		makeGetterAndSetter(m_SpriteTexturename, SpriteTextureName);

		#define __gettersettertype GLfloat
		makeGetterAndSetter(m_SpriteRotation,    SpriteRotation);

		#undef __gettersettertype

		void render(void) const noexcept;

	private:
		glm::vec2 m_SpritePosition;
		glm::vec2 m_SpriteSize;
		glm::vec3 m_SpriteColor;
		
		std::string m_SpriteTexturePath;
		std::string m_SpriteTextureName;
		GLfloat     m_SpriteRotation;
	};
}