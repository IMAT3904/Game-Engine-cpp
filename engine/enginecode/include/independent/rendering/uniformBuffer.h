#pragma once
/* \file uniformBuffer.h */
#include <ctype.h>
#include <unordered_map>
#include <memory>

#include"bufferLayout.h"
#include "shader.h"
namespace Engine
{
	/**
	\class UniformBuffer
	\brief Api agnostic uniform buffer.
	*/
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default; //!< Destructor.
		virtual inline uint32_t GetRenderID() = 0; //!< Get ID.
		virtual inline UniformBufferLayout GetLayout() = 0; //!< Get buffer Layout.
		virtual void attachShaderBlock(const std::shared_ptr<Shader> shader, const char* blockName) = 0; //!< Attach a shader block.
		virtual void uploadData(const char* uniformName, void* data)= 0; //!< Upload data to buffer.

		static UniformBuffer* create(const UniformBufferLayout layout); //!< Create a buffer
	protected:
		UniformBufferLayout m_layout; //!< Uniform Buffer layout
		std::unordered_map<const char*, std::pair<uint32_t, uint32_t>> m_uniformChache; //!< Stores uniform names with offsets and sizes
		uint32_t m_blockNumber; //!< Block number for this UBO
	};
}