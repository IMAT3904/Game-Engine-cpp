#pragma once
/* \file OpenGLUniformBuffer.h */

#include "rendering/uniformBuffer.h"

namespace Engine
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const UniformBufferLayout& layout); //!< Constructor
		~OpenGLUniformBuffer() override; //!< Destructor
		inline uint32_t GetRenderID() override  { return m_OpenGL_ID; };
		inline UniformBufferLayout GetLayout() override { return m_layout; };
		void attachShaderBlock(const std::shared_ptr<Shader> shader, const char * blockName) override;
		void uploadData(const char* uniformName, void* data) override;
	private:
		uint32_t m_OpenGL_ID; //!< OpenGL ID
		static uint32_t s_blockNumber; //!< Global block number
	};
}