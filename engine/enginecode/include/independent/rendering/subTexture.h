/** \file subTexture.h */
#pragma once

#include "platform/OpenGL/OpenGLTexture.h"
#include "rendering/texture.h"
#include <memory>
#include <glm/glm.hpp>
namespace Engine
{
	/**
	\class SubTexture
	\brief Api angostic Subtexture class. Can slice part of texture.
	*/
	class SubTexture
	{
	public:
		SubTexture(); //!< Constructor.
		SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& UVStart, glm::vec2 UVEND); //!< Constructor with parameters.
		inline glm::vec2 getUVStart() const { return m_UVStart; }; //!< Get UV start coordinates.
		inline glm::vec2 getUVEnd()const { return m_UVEnd; }; //!< Get UV end coordinates.
		glm::ivec2 getSize() { return m_size; }; //!< Get the size of subtexture.
		glm::vec2 getSizef() { return { static_cast<float>(m_size.x), static_cast<float>(m_size.y) }; }; //!< Get the size of subtexture as floats.
		inline uint32_t getWidth() { return m_size.x; } //!< Get width of subtexture.
		inline uint32_t getWidthf() { return  static_cast<float>(m_size.x); } //!< Get width of subtexture as floats.
		inline uint32_t getHeightf() { return static_cast<float>(m_size.y); } //!< Get height of subtexture as floats
		inline uint32_t getHeight() { return m_size.y; } //!< Get heigh of subtexture.
		float transformU(float U); //!< Transform original U to atlased co-ords.
		float transformV(float V); //!< Transform original V to atlased co-ords.
		glm::vec2 transformUV(glm::vec2 UV); //!< Transform original UV to atlased co-ords.
		inline std::shared_ptr<Texture> getBaseTexture()const  { return m_texture; }; //!< Get Base texture.
	private:
		std::shared_ptr<Texture> m_texture; //!< Pointer to base texture.
		glm::vec2 m_UVStart; //!< UV start.
		glm::vec2 m_UVEnd; //!< UV end.
		glm::vec2 m_size; //!< Size in pixels
	};
}