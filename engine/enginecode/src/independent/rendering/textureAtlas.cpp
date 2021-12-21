/* \file textureAtlass.cpp */

#include "engine.h"
#include "rendering/textureAtlas.h"
#include <stb_image.h>

namespace Engine
{
	TextureAtlas::TextureAtlas(glm::ivec2 size, uint32_t channels, uint32_t reservedSpaces)
	{
		m_baseTexture.reset(Texture::create(size.x, size.y, channels, nullptr));
		m_spaces.reserve(reservedSpaces);
		m_spaces.push_back({ 0,0,size.x,size.y });
	}
	bool TextureAtlas::add(const char* filepath, SubTexture& result)
	{
		int32_t width, height, channels;
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, static_cast<int>(getChannels()));
		
		if (data) return add(width, height, channels, data, result);
		
		return false;
	}
	bool TextureAtlas::add(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data, SubTexture& result)
	{
		if (channels != getChannels())
			return false; //Early exit as channels don't match

		for (auto it = m_spaces.begin(); it != m_spaces.end(); ++it)
		{
			auto& space = *it;
			//Does the texture fit this space
			if (width < space.w && height < space.h)
			{
				//texture fits! Add texture data
				m_baseTexture->edit(space.x, space.y, width, height, data);

				//Set subtexture result
				glm::vec2 UVStart(static_cast<float>(space.x) / m_baseTexture->getWidthf(), static_cast<float>(space.y) / m_baseTexture->getHeightf());
				glm::vec2 UVEnd(static_cast<float>(space.x + space.w) / m_baseTexture->getWidthf(), static_cast<float>(space.y+space.h) / m_baseTexture->getHeightf());

				result = SubTexture(m_baseTexture, UVStart, UVEnd);

				//Sort out remaining spaces


				//Case 1 texture matches space size, delete space
				if (width == space.w && height == space.h)
				{
					m_spaces.erase(it);
					return true;
				}


				//Case 2 texture width matches space width, height do not match - split the space horizontally in two
				//
				// -----------			----------
				// |         |  becomes |        |
				// | Space   |			| -------|
				// |         |			|  Space |
				// -----------			----------
				else if (width == space.w)
				{
					space.y += height;
					space.h -= height;
					return true;
				}

				//Case 3 texture width matches height, width do not match - split the space vertically in two
				//
				// -----------			----------
				// |         |  becomes |   |Sp  |
				// | Space   |			|   |ac  |
				// |         |			|   |e   |
				// -----------			----------

				else if (height == space.h)
				{
					space.x += width;
					space.w -= width;
					return true;
				}

				//Case 4 Texture height and width do not match = split the space horizontally and vertically
				//
				// -----------			----------
				// |         |  becomes |   |Sp  |
				// | Space   |			|---|ac  |
				// |         |			|Spa|e   |
				// -----------			----------
				else
				{
					m_spaces.push_back({ space.x,space.y + height,width,space.h - height });

					space.x += width;
					space.w -= width;
					return true;
				}
			}
		}

		return false;
	}
}