/** \file texture.h */
#pragma once
#include <cstdint>

namespace Engine
{
	/**
	\class Texture
	\brief Api agnostic Texture class.
	*/
	class Texture
	{
	public:
		virtual ~Texture() = default; //!< Default constructor
		virtual inline  uint32_t getID() const = 0; //!< Get texture Id.

		virtual void edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data) const = 0; //!< Edit texture
		virtual inline uint32_t getWidth() const = 0; //!< Get width of texture.
		virtual inline uint32_t getWidthf() const = 0; //!< Get width of texture as float.
		virtual inline uint32_t getHeightf() const = 0; //!< Get height of texture as float. 
		virtual inline uint32_t getHeight() const = 0; //!< Get height of texture.
		virtual inline uint32_t getChannels() const = 0; //!< Get number of channels.


		static Texture* create(const char* filepath); //!< Create texture from given path.
		static Texture* create(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data); //Create texture from given parameters.
	};
}