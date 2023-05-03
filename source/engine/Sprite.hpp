#pragma once

#include "string"

#include "utility/GetSetMacro.hpp"
#include "rendering/SpriteRenderer.hpp"
#include "rendering/TextureWrapper.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace Engine::GFX
{
	class Sprite
	{
	public:
		Sprite() : m_SpritePosition({ 0.0f, 0.0f }), m_SpriteSize({ 0.0f, 0.0f }), m_SpriteColor({ 1.0f, 1.0f, 1.0f }), m_SpriteRotation(0.0f)
		{
		}

		void inline bindTexture(const std::string& textureName) noexcept
		{
			m_BindedTextureName = textureName;
		}
		 
		#define __gettersettertype glm::vec2
		makeGetterAndSetter(m_SpritePosition, SpritePosition);
		makeGetterAndSetter(m_SpriteSize,     SpriteSize);

		#define __gettersettertype glm::vec3
		makeGetterAndSetter(m_SpriteColor, SpriteColor);
		
		#define __gettersettertype GLfloat		
		makeGetterAndSetter(m_SpriteRotation,    SpriteRotation);
		#undef __gettersettertype

		void render(std::shared_ptr<Engine::GFX::SpriteRenderer>& spriteRenderer) const noexcept;

	private:
		glm::vec2 m_SpritePosition;
		glm::vec2 m_SpriteSize;
		glm::vec3 m_SpriteColor;
		GLfloat   m_SpriteRotation;

		std::string m_BindedTextureName;
	};
}