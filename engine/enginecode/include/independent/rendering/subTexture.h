/** \file subTexture.h */
#pragma once

#include "platform/OpenGL/OpenGLTexture.h"
#include "rendering/texture.h"
#include <memory>
#include <glm/glm.hpp>
namespace Engine
{
	class SubTexture
	{
	public:
		SubTexture();
		SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& UVStart, glm::vec2 UVEND);
		inline glm::vec2 getUVStart() { return m_UVStart; };
		inline glm::vec2 getUVEnd() { return m_UVEnd; };
		glm::ivec2 getSize() { return m_size; };
		glm::vec2 getSizef() { return { static_cast<float>(m_size.x), static_cast<float>(m_size.y) }; };
		inline uint32_t getWidth() { return m_size.x; }
		inline uint32_t getWidthf() { return  static_cast<float>(m_size.x); }
		inline uint32_t getHeightf() { return static_cast<float>(m_size.y); }
		inline uint32_t getHeight() { return m_size.y; }
		float transformU(float U); //!< Transform original to atlased co-ords
		float transformV(float V);
		glm::vec2 transformUV(glm::vec2 UV);
		inline std::shared_ptr<Texture> getBaseTexture()const  { return m_texture; };
	private:
		std::shared_ptr<Texture> m_texture;
		glm::vec2 m_UVStart;
		glm::vec2 m_UVEnd;
		glm::vec2 m_size; //!< Size in pixels
	};
}