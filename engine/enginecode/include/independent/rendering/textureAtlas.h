/*\file textureAtlas.h */
#pragma once

#include <vector>
#include "rendering/subTexture.h"

namespace Engine
{
	struct SimpleRect { int32_t x, y, w, h; };

	/**
	\class TextureAtlas
	\brief Texture atlas (texture of texures).
	*/
	class TextureAtlas
	{
	public:
		TextureAtlas(glm::ivec2 size = { 4096,4096 }, uint32_t channels = 4, uint32_t reservedSpaces =32); //!< Constructor, takes size and channels
		bool add(const char* filepath, SubTexture& result); //!< Attempt to add a subtexture with a file
		bool add(int32_t width, int32_t height, uint32_t channels, unsigned char * data, SubTexture& result); //!< Attempt to add a subtexture with a file
		inline uint32_t getChannels() { return m_baseTexture->getChannels(); } //!< Get number of channels.
		inline std::shared_ptr<Texture> getBaseTexture() const { return m_baseTexture; } //!<Get Base Texture.
		inline uint32_t getID() const { return m_baseTexture->getID(); } //Get base texture Id
	private:
		std::vector<SimpleRect> m_spaces; //!< Gaps left which can be filled
		std::shared_ptr<Texture> m_baseTexture; //!< texture which hold all subtexture pixel data
	};
}