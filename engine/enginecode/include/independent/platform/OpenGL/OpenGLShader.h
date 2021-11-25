/** \file OpenGLShader.h */
#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "rendering/shader.h"

namespace Engine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexFilePath, const char* fragmentFilePath);
		OpenGLShader(const char* filepath);
		~OpenGLShader();
		virtual inline uint32_t getID() const override { return m_OpenGL_ID; }

		virtual void uploadInt(const char* name, int value) const override;
		virtual void uploadFloat(const char* name, float value) const override;
		virtual void uploadFloat2(const char* name, const glm::vec2& value) const override;
		virtual void uploadFloat3(const char* name, const glm::vec3& value) const override;
		virtual void uploadFloat4(const char* name, const glm::vec4& value) const override;
		virtual void uploadMat4(const char* name, const glm::mat4& value) const override;
	private:
		uint32_t m_OpenGL_ID;
		void compileAndLink(const char* vertexShaderSrc, const char* fragmentShaderSrc);
	};
}
