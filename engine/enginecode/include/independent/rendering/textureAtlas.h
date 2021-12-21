/*\file textureAtlas.h */
#pragma once

#include <vector>
#include "rendering/subTexture.h"

namespace Engine
{
	struct SimpleRect { uint32_t x, y, w, h; };

	class TextureAtlas
	{
	public:
		TextureAtlas(glm::ivec2 size = { 4096,4096 }, uint32_t channels = 4, uint32_t reservedSpaces =32); //!< Constructor, takes size and channels
		bool add(const char* filepath, SubTexture& result); //!< Attempt to add a subtexture with a file
		bool add(uint32_t width, uint32_t height, uint32_t channels, unsigned char * data, SubTexture& result); //!< Attempt to add a subtexture with a file
		inline uint32_t getChannels() { m_baseTexture->getChannels(); }
		inline std::shared_ptr<Texture> getBaseTexture() const { return m_baseTexture; }
		inline uint32_t getID() const { return m_baseTexture->getID(); }
	private:
		std::vector<SimpleRect> m_spaces; //!< Gaps left which can be filled
		std::shared_ptr<Texture> m_baseTexture; //!< texture which hold all subtexture pixel data
	};
}