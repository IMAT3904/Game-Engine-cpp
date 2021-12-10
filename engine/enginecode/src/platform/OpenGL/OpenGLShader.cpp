/** \file OpenGLShader.cpp */

#include "engine_pch.h"

#include <fstream>
#include <string>
#include <array>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "platform/OpenGL/OpenGLShader.h"
#include "systems/log.h"

namespace Engine {
	OpenGLShader::OpenGLShader(const char* vertexFilePath, const char* fragmentFilePath)
	{
		std::string line, vertexSrc, fragmentSrc;

		std::fstream handle(vertexFilePath, std::ios::in);
		if (handle.is_open())
		{
			while (getline(handle, line))
			{
				vertexSrc += (line + "\n");
			}
		}
		else
		{
			Log::error("Could not open vertex shader source : {0}", vertexFilePath);
			return;
		}
		handle.close();
		
		handle.open(fragmentFilePath, std::ios::in);
		if (handle.is_open())
		{
			while (getline(handle, line))
			{
				vertexSrc += (line + "\n");
			}
		}
		else
		{
			Log::error("Could not open vertex shader source : {0}", vertexFilePath);
			return;
		}

		handle.close();
		compileAndLink(vertexSrc.c_str(), fragmentSrc.c_str());
	}
	OpenGLShader::OpenGLShader(const char* filepath)
	{
		enum Region {None = -1, Vertex = 0, Fragment, Geometry, TessellationControl, TesselationEvaluation, Compute} ;
		std::string line;
		std::array<std::string, Compute + 1> src;

		uint32_t region = Region::None;



		std::fstream handle(filepath, std::ios::in);
		if (handle.is_open())
		{
			while (getline(handle, line))
			{
				if (line.find("#region Vertex") != std::string::npos) { region = Region::Vertex; continue; };
				if (line.find("#region Fragment") != std::string::npos) {region = Region::Fragment; continue;}
				if (line.find("#region Geometry") != std::string::npos) { region = Region::Geometry; continue; }
				if (line.find("#region TessellationControl") != std::string::npos) { region = Region::TessellationControl; continue;}
				if (line.find("#region TesselationEvaluation") != std::string::npos) { region = Region::TesselationEvaluation; continue; }
				if (line.find("#region Compute") != std::string::npos) { region = Region::Compute; continue; }


				if(region != Region::None)
					src[region] += (line + "\n");
			}
		}
		else
		{
			Log::error("Could not open shader file : {0}", filepath);
			return;
		}
		handle.close();
		compileAndLink(src[Region::Vertex].c_str(), src[Region::Fragment].c_str());
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_OpenGL_ID);
	}
	void OpenGLShader::uploadIntArray(const char* name, int32_t* values, uint32_t count) const
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform1iv(uniformLocation, count, values);
	}
	void  OpenGLShader::uploadInt(const char* name, int value) const
	{
		uint32_t unifromLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform1i(unifromLocation, value);
	}
	void OpenGLShader::uploadFloat(const char* name, float value) const
	{
		uint32_t unifromLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform1f(unifromLocation, value);
	}
	void OpenGLShader::uploadFloat2(const char* name, const glm::vec2& value) const
	{
		uint32_t unifromLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform2f(unifromLocation, value.x, value.y);
	}
	void OpenGLShader::uploadFloat3(const char* name, const glm::vec3& value) const
	{
		uint32_t unifromLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform3f(unifromLocation, value.x,value.y,value.z);
	}
	void OpenGLShader::uploadFloat4(const char* name, const glm::vec4& value) const
	{
		uint32_t unifromLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniform4f(unifromLocation, value.x, value.y, value.z,value.w);
	}
	void OpenGLShader::uploadMat4(const char* name, const glm::mat4& value) const
	{
		uint32_t unifromLocation = glGetUniformLocation(m_OpenGL_ID, name);
		glUniformMatrix4fv(unifromLocation, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::compileAndLink(const char* vertexShaderSrc, const char* fragmentShaderSrc)
	{
		
		uint32_t TPprogram;

		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = vertexShaderSrc;
		glShaderSource(vertShader, 1, &source, 0);
		glCompileShader(vertShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(vertShader);
			return;
		}

		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentShaderSrc;
		glShaderSource(fragShader, 1, &source, 0);
		glCompileShader(fragShader);

		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(fragShader);
			glDeleteShader(vertShader);

			return;
		}

		m_OpenGL_ID = glCreateProgram();
		glAttachShader(m_OpenGL_ID, vertShader);
		glAttachShader(m_OpenGL_ID, fragShader);
		glLinkProgram(m_OpenGL_ID);

		GLint isLinked = 0;
		glGetProgramiv(m_OpenGL_ID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_OpenGL_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_OpenGL_ID, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_OpenGL_ID);
			glDeleteShader(vertShader);
			glDeleteShader(fragShader);

			return;
		}

		glDetachShader(m_OpenGL_ID, vertShader);
		glDetachShader(m_OpenGL_ID, fragShader);
	}
}