#pragma once
/* \file vertexBuffer.h */

#include <cstdint>
#include "rendering/bufferLayout.h"

namespace Engine
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual inline  uint32_t getRenderID() const = 0;

	
		virtual void edit(void* vertices, uint32_t size, uint32_t offset)const = 0;
		virtual inline const BufferLayout& getLayout()const = 0;

		static VertexBuffer* create(void* vertices, uint32_t size, BufferLayout layout);
	};
}
