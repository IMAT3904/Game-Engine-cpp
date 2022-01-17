/** \file shader.h */
#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace Engine 
{
	/**
	\class Shader
	\brief API agnostic shader class.
	*/
	class Shader
	{
	public:
		virtual ~Shader() = default; //!< Destructor.
		virtual inline  uint32_t getID() const = 0; //!< Get shader ID.

		static Shader* create(const char* vertexFilePath, const char* fragmentFilePath); //!< Create shader from given paths to vertex and fragment shader.
		static Shader* create(const char* filepath); //!< Create shader from given path to combined shader.

		virtual void uploadInt(const char* name, int value) const =0; //!< Upload integer to shader.
		virtual void uploadIntArray(const char* name, int32_t* values, uint32_t count) const = 0; //!< Upload array of integers to shader.
		virtual void uploadFloat(const char* name, float value)const = 0; //!< Upload float to shader. 
		virtual void uploadFloat2(const char* name, const glm::vec2& value)const = 0; //!< Upload vector of 2 floats to shader.
		virtual void uploadFloat3(const char* name, const glm::vec3& value)const = 0; //!< Upload vector of 3 floats to shader.
		virtual void uploadFloat4(const char* name, const glm::vec4& value)const = 0; //!< Upload vector of 4 floats to shader.
		virtual void uploadMat4(const char* name, const glm::mat4& value)const = 0; //!< Upload 4x4 matrix to shader.
	};
}