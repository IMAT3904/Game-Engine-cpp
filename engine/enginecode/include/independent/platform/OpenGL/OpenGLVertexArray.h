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
		/**
		\class OpenGLVertexArray
		\brief OpenGL implementation of Vertex Array.
		*/

	public:
		OpenGLVertexArray(); //!< Constructor
		~OpenGLVertexArray(); //!< Destructor
		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)  override; //!< Add vertex buffer.
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override; //!< Set index buffer.
		inline uint32_t getID() const override  { return m_OpenGL_ID; } //!< Get OpenGL ID.
		virtual inline std::shared_ptr<IndexBuffer> getIndexBuffer()const override { return m_indexBuffer; }//!< Get index buffer.
		inline uint32_t getDrawCount() const override //!< Get draw count.
		{
			if (m_indexBuffer) { m_indexBuffer->getCount(); }
			else { return 0; }
		}
		inline std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() const override { return m_vertexBuffers; } //!< Get vertex buffers.
	private:
		uint32_t m_OpenGL_ID; //!< Render ID
		uint32_t m_attributeIndex = 0; //!< Atribute Index
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers; //!< Vector of pointers to Vertex buffers.
		std::shared_ptr<IndexBuffer> m_indexBuffer; //!< Pointer to Index buffer.
	};
}