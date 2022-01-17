/** \file OpenGLShader.h */
#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "rendering/shader.h"

namespace Engine
{
	/**
	\class OpenGLShader
	\brief OpenGL implementation of shader class.
	*/
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexFilePath, const char* fragmentFilePath);//!< Constructor which takes path to fragment and vertex shaders.
		OpenGLShader(const char* filepath); //!< Constructor which takes only path to combined shader file.
		~OpenGLShader(); //!< Destructor.
		virtual inline uint32_t getID() const override { return m_OpenGL_ID; } //!< Get OpenGL ID.
		virtual void uploadIntArray(const char* name, int32_t* values, uint32_t count) const override; //!< Upload to shader an array of integers. 
		virtual void uploadInt(const char* name, int value) const override; //!< Upload to shader single integer value.
		virtual void uploadFloat(const char* name, float value) const override; //!< Upload to shader floating point value.
		virtual void uploadFloat2(const char* name, const glm::vec2& value) const override; //!< Upload to shader vector of 2 floats.
		virtual void uploadFloat3(const char* name, const glm::vec3& value) const override; //!< Upload to shader vector of 3 floats.
		virtual void uploadFloat4(const char* name, const glm::vec4& value) const override; //!< Upload to shader vector of 4 floats.
		virtual void uploadMat4(const char* name, const glm::mat4& value) const override; //!< Upload to shader 4x4 matrix.
	private:
		uint32_t m_OpenGL_ID; //!< Holds an OpenGL ID.
		void compileAndLink(const char* vertexShaderSrc, const char* fragmentShaderSrc); //!< Compile and link the shader (preparation).
	};
}
