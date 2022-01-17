#pragma once
/* \file vertexArray.h */

#include <cstdint>
#include "rendering/indexBuffer.h"
#include "rendering/vertexBuffer.h"
#include <memory>

namespace Engine
{
	/**
	\class VertexArray
	\brief Api agnostic Vertex array.
	*/
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default; //!< Constructor.
		virtual inline  uint32_t getID() const = 0; //!< Get ID.
		virtual inline std::shared_ptr<IndexBuffer> getIndexBuffer() const = 0; //!< Get Index buffer.
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0; //!< Add vertex buffer.
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)  =0; //!< Set index buffer.
		virtual inline uint32_t getDrawCount() const = 0; //!< Get the draw count.
		virtual inline std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() const = 0; //!< Get all vertex buffers.

		static VertexArray* create(); //!< Create vertex array.
	};
}