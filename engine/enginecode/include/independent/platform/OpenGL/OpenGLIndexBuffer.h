/** \file OpenGLIndexBuffer.h */
#pragma once

#include "rendering/indexBuffer.h"

namespace Engine
{
	/**
	\class OpenGLIndexBuffer
	\brief OpenGL implementation of index buffer.
	*/
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indicies, uint32_t count); //!< Constructor
		virtual ~OpenGLIndexBuffer(); //!< Destructor
		virtual inline uint32_t getID() const override { return m_OpenGL_ID; } //!< Get the OpenGL ID.
		virtual inline uint32_t getCount() const override  { return m_count; } //!< Get the draw count.
	private:
		uint32_t m_OpenGL_ID; //!< Render ID
		uint32_t m_count; //!< Effective draw count
	};
}