/** \file GLFWWindowImpl.h */

#include "engine_pch.h"
#include<glad/glad.h>
#include "platform/OpenGL/OpenGLVertexBuffer.h"

Engine::OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size, BufferLayout layout) :m_layout(layout)
{
	glCreateBuffers(1, &m_OpenGL_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);

}

Engine::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_OpenGL_ID);
}

void Engine::OpenGLVertexBuffer::edit(void* vertices, uint32_t size, uint32_t offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_ID);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
}
