/** \file OpenGLVertexBuffer.h */
#pragma once

#include "rendering/bufferLayout.h"
#include "rendering/vertexBuffer.h"
namespace Engine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
		/**
		\class OpenGLVertexBuffer
		\brief OpenGL implementation of Vertex Buffer.
		*/
	public:
		OpenGLVertexBuffer(void* vertices, uint32_t size, VertexBufferLayout layout); //!< Constructor
		~OpenGLVertexBuffer(); //!< Destructor
		void edit(void* vertices, uint32_t size, uint32_t offset) const override; //!< Edit vertices at given offset.
		inline uint32_t getRenderID() const override { return m_OpenGL_ID; } //!< Get Render ID.
		inline const VertexBufferLayout& getLayout() const override { return m_layout; } //!< Get the vertex buffer layout.
	private:
		uint32_t m_OpenGL_ID; //!< Render ID.
		VertexBufferLayout m_layout; //!< Vertex buffer layout.
	};
}