#pragma once
/** \file OpenGLTexture.h */

#include "rendering/texture.h"
namespace Engine
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const char* filepath);
		OpenGLTexture(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data);
		~OpenGLTexture();
		virtual void edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data) const override;
		virtual inline uint32_t getID() const override { return m_OpenGL_ID;  }
		virtual inline uint32_t getWidth() const override  { return m_width;  }
		virtual inline uint32_t getWidthf() const override { return  static_cast<float>(m_width);  }
		virtual inline uint32_t getHeightf() const override { return static_cast<float>(m_height);  }
		virtual inline uint32_t getHeight() const override { return m_height;  }
		virtual inline uint32_t getChannels() const override { return m_channels;  }
	private:
		uint32_t m_OpenGL_ID;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_channels;
		void init(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data);

	};

}