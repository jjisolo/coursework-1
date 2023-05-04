// This file decalres the `Sprite` class.
#pragma once

#include "_EngineIncludes.hpp"

#include "rendering/SpriteRenderer.hpp"
#include "rendering/TextureWrapper.hpp"

// This namespace is populated with all graphics-related stuff.
namespace Engine::GFX
{
	// This class represents a easily-drawable texture.
	// 
	// This class purpose is to simplify the render routine of a texture.
	class Sprite
	{
	public:
		Sprite() : m_SpritePosition({ 0.0f, 0.0f }), m_SpriteSize({ 0.0f, 0.0f }), m_SpriteColor({ 1.0f, 1.0f, 1.0f }), m_SpriteRotation(0.0f)
		{
		}

		// Bind the ::TextureWrapper descriptor to this sprite.
		void inline bindTexture(const std::string& textureName) noexcept
		{
			m_BindedTextureName = textureName;
		}
		 
		// Getters and setters for the sprite data.
		#define __gettersettertype glm::vec2
		makeGetterAndSetter(m_SpritePosition, SpritePosition);
		makeGetterAndSetter(m_SpriteSize,     SpriteSize);

		#define __gettersettertype glm::vec3
		makeGetterAndSetter(m_SpriteColor, SpriteColor);
		
		#define __gettersettertype GLfloat		
		makeGetterAndSetter(m_SpriteRotation,    SpriteRotation);
		#undef __gettersettertype

		// Render the sprite on the screen using the ::SpriteRenderer tool.
		void render(std::shared_ptr<Engine::GFX::SpriteRenderer>& spriteRenderer) const noexcept;

	private:
		glm::vec2 m_SpritePosition;
		glm::vec2 m_SpriteSize;
		glm::vec3 m_SpriteColor;
		GLfloat   m_SpriteRotation;

		std::string m_BindedTextureName;
	};
}