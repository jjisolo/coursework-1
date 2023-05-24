// This file decalres the `Sprite` class.
#pragma once

#include "_EngineIncludes.hpp"

#include "rendering/SpriteRenderer.hpp"
#include "rendering/TextureWrapper.hpp"

using namespace std;
using namespace glm;

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

		// Getters and setters for the sprite data.
		#define __gettersettertype glm::vec2
		makeGetterAndSetter(m_SpritePosition, SpritePosition);
		makeGetterAndSetter(m_SpriteSize,     SpriteSize);

		#define __gettersettertype glm::vec3
		makeGetterAndSetter(m_SpriteColor, SpriteColor);
		
		#define __gettersettertype GLuint
		makeGetterAndSetter(m_RenderFlag, RenderFlag);

		#define __gettersettertype GLfloat		
		makeGetterAndSetter(m_SpriteRotation,    SpriteRotation);
		#undef __gettersettertype

		// Render the sprite on the screen using the ::SpriteRenderer tool.
		void render(shared_ptr<Engine::GFX::SpriteRenderer>& spriteRenderer) const noexcept;

        // Bind the ::TextureWrapper descriptor to this sprite.
        void bindTexture(const string& textureName) noexcept;

        // Find out if the sprite is hovered by the mouse.
        bool isHovered(vec2 mousePosition, vec2 range) const;

	protected:
		vec2      m_SpritePosition;
        vec2      m_SpriteSize;
		vec3      m_SpriteColor;
		GLfloat   m_SpriteRotation;

		GLuint m_RenderFlag;
		string m_BindedTextureName;
	};
}
