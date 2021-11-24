/* file renderer2D.cpp*/

#include "engine_pch.h"
#include <glad/glad.h>
#include "rendering/Renderer2D.h"
namespace Engine
{
	std::shared_ptr<Renderer2D::InternalData> Renderer2D::s_data = nullptr;

	void Renderer2D::init()
	{
		s_data.reset(new InternalData);

		unsigned char whitePX[4] = { 255,255,255,255 };
		s_data->defaultTexture.reset(Texture::create(1, 1, 4, whitePX));

		s_data->defaultTint = { 1.f,1.f,1.f,1.f };
		s_data->model = glm::mat4(1.0f);

		s_data->shader.reset(Shader::create("./assets/shaders/quad1.glsl"));

		float vertices[4 * 4] =
		{
			-0.5f, -0.5f, 0.f, 0.f,
			-0.5f,  0.5f, 0.f, 1.f,
			 0.5f,  0.5f, 1.f, 1.f,
			 0.5f,  -0.5f, 1.f, 0.f
		};
		uint32_t indices[4] = { 0,1,2,3 };

		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<IndexBuffer> IBO;

		s_data->VAO.reset(VertexArray::create());
		VBO.reset(VertexBuffer::create(vertices, sizeof(vertices), BufferLayout({ ShaderDataType::Float2, ShaderDataType::Float2 })));
		IBO.reset(IndexBuffer::create(indices, 4));
		s_data->VAO->addVertexBuffer(VBO);
		s_data->VAO->setIndexBuffer(IBO);
	}
	void Renderer2D::begin(const SceneWideUniforms& swu)
	{
		//Bind the shader
		glUseProgram(s_data->shader->getID());

		//Apply sceneWideUniforms
		for (auto& dataPair : swu)
		{
			const char* nameOfUniform = dataPair.first;
			ShaderDataType sdt = dataPair.second.first;
			void* addressOfValue = dataPair.second.second;

			switch (sdt)
			{
			case ShaderDataType::Int:
				s_data->shader->uploadInt(nameOfUniform, *(int*)addressOfValue);
				break;
			case ShaderDataType::Float3:
				s_data->shader->uploadFloat3(nameOfUniform, *(glm::vec3*)addressOfValue);
				break;
			case ShaderDataType::Float4:
				s_data->shader->uploadFloat4(nameOfUniform, *(glm::vec4*)addressOfValue);
				break;
			case ShaderDataType::Mat4:
				s_data->shader->uploadMat4(nameOfUniform, *(glm::mat4*)addressOfValue);
				break;
			}
		}

		//Bind the geometry
		glBindVertexArray(s_data->VAO->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->VAO->getIndexBuffer()->getID());
	}
	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint)
	{
	}
	void Renderer2D::end()
	{
	}
}