/** \file shader.h */
#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace Engine 
{
	class Shader
	{
	public:
		virtual ~Shader() = default;
		virtual inline  uint32_t getID() const = 0;

		static Shader* create(const char* vertexFilePath, const char* fragmentFilePath);
		static Shader* create(const char* filepath);

		virtual void uploadInt(const char* name, int value) const =0;
		virtual void uploadIntArray(const char* name, int32_t* values, uint32_t count) const = 0;
		virtual void uploadFloat(const char* name, float value)const = 0;
		virtual void uploadFloat2(const char* name, const glm::vec2& value)const = 0;
		virtual void uploadFloat3(const char* name, const glm::vec3& value)const = 0;
		virtual void uploadFloat4(const char* name, const glm::vec4& value)const = 0;
		virtual void uploadMat4(const char* name, const glm::mat4& value)const = 0;
	};
}