#pragma once
/** \file OpenGLTexture.h */

#include <cstdint>
namespace Engine
{
	class OpenGLTexture
	{
	public:
		OpenGLTexture(const char* filepath);
		OpenGLTexture(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data);
		~OpenGLTexture();
		void edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data);
		inline uint32_t getID() { return m_OpenGL_ID;  }
		inline uint32_t getWidth() { return m_width;  }
		inline uint32_t getWidthf() { return  static_cast<float>(m_width);  }
		inline uint32_t getHeightf() { return static_cast<float>(m_height);  }
		inline uint32_t getHeight() { return m_height;  }
		inline uint32_t getChannels() { return m_channels;  }
	private:
		uint32_t m_OpenGL_ID;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_channels;
		void init(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data);

	};

}