#pragma once
/** \file OpenGLTexture.h */

#include "rendering/texture.h"
namespace Engine
{
	/**
	\class OpenGLTexture
	\brief OpenGL implementation of a Texture class.
	*/
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const char* filepath); //!< Constructor that takes file path to a texture. 
		OpenGLTexture(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data); //!< Constructor that takes all necessary texture parameters.
		~OpenGLTexture(); //!< Destructor
		virtual void edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data) const override; //!< Edit the texture.
		virtual inline uint32_t getID() const override { return m_OpenGL_ID;  } //!< Get the texture ID.
		virtual inline uint32_t getWidth() const override  { return m_width;  } //!< Get the texture width.
		virtual inline uint32_t getWidthf() const override { return  static_cast<float>(m_width);  } //!<Get the texture width as float.
		virtual inline uint32_t getHeightf() const override { return static_cast<float>(m_height);  } //!< Get the texture height as float.
		virtual inline uint32_t getHeight() const override { return m_height;  } //!< Get the texture height.
		virtual inline uint32_t getChannels() const override { return m_channels;  } //!< Get the number of channels.
	private:
		uint32_t m_OpenGL_ID; //!<Opengl ID
		uint32_t m_width; //!< Texture width.
		uint32_t m_height; //!< Texture height.
		uint32_t m_channels; //!< Number of channels.
		void init(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data); //!< Initializes a texture.

	};

}