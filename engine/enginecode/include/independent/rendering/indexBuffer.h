/** \file indexBuffer.h */
#pragma once

#include<cstdint>

namespace Engine
{
	/** \class IndexBuffer
	* API diagnostic index bufffer
	*/
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;
		virtual inline uint32_t getRenderID() const = 0;
		virtual inline uint32_t getCount() const = 0;

		static IndexBuffer* create(uint32_t* indicies, uint32_t count);
	};
}
