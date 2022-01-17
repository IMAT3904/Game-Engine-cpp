#pragma once
/* \file vertexBuffer.h */

#include <cstdint>
#include "rendering/bufferLayout.h"

namespace Engine
{
	/**
	\class VertexArray
	\brief Api agnostic Vertex Buffer.
	*/
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default; //!< Default constructor.
		virtual inline  uint32_t getRenderID() const = 0; //!< Get Render ID.

	
		virtual void edit(void* vertices, uint32_t size, uint32_t offset)const = 0; //!< Edit the vertex buffer.
		virtual inline const VertexBufferLayout& getLayout()const = 0; //!< Get the layout.

		static VertexBuffer* create(void* vertices, uint32_t size, const VertexBufferLayout layout); //!< Create the vertex buffer.
	};
}
