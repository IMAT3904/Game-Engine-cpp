#pragma once
/* \file OpenGLUniformBuffer.h */

#include "rendering/uniformBuffer.h"

namespace Engine
{
	/**
	\class OpenGLUniformBuffer
	\brief OpenGL implementation of Uniform Buffer.
	*/

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const UniformBufferLayout& layout); //!< Constructor
		~OpenGLUniformBuffer() override; //!< Destructor
		inline uint32_t GetRenderID() override  { return m_OpenGL_ID; }; //!< Get OpenGlID
		inline UniformBufferLayout GetLayout() override { return m_layout; }; //!< Get layout of the buffer.
		void attachShaderBlock(const std::shared_ptr<Shader> shader, const char * blockName) override; //!< Attatch shader block.
		void uploadData(const char* uniformName, void* data) override; //!< Upload data.
	private:
		uint32_t m_OpenGL_ID; //!< OpenGL ID
		static uint32_t s_blockNumber; //!< Global block number
	};
}