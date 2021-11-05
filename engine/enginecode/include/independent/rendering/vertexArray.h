#pragma once
/* \file vertexArray.h */

#include <cstdint>
#include "rendering/indexBuffer.h"
#include <memory>

namespace Engine
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;
		virtual inline  uint32_t getRenderID() const = 0;
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)  =0;
		virtual inline uint32_t getDrawCount() const = 0;

		static VertexArray* create();
	};
}