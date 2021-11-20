/* \file OpenGLVertexArray.h */
#pragma once

#include <vector>
#include <memory>
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"
#include "rendering/vertexArray.h"

namespace Engine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)  override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		inline uint32_t getID() const override  { return m_OpenGL_ID; }
		virtual inline std::shared_ptr<IndexBuffer> getIndexBuffer()const override { return m_indexBuffer; }
		inline uint32_t getDrawCount() const override  
		{
			if (m_indexBuffer) { m_indexBuffer->getCount(); }
			else { return 0; }
		}
	private:
		uint32_t m_OpenGL_ID; //!< Render ID
		uint32_t m_attributeIndex = 0; //!< Atribute Index
		std::vector<std::shared_ptr<IndexBuffer>> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};
}