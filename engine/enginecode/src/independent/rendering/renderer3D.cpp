/** \file renderer3d.cpp*/

#include "engine_pch.h"
#include "rendering/Renderer3D.h"
#include "glad/glad.h"

namespace Engine
{
	std::shared_ptr<Renderer3D::InternalData> Renderer3D::s_data = nullptr;
	void Renderer3D::init()
	{
		s_data.reset(new InternalData);

		unsigned char whitePX[4] = { 255,255,255,255 };
		s_data->defaultTexture.reset(Texture::create(1, 1, 4, whitePX));
		s_data->defaultTint = { 1.f,1.f,1.f,1.f };

	}
	void Renderer3D::begin(const SceneWideUniforms& sceneWideUniforms)
	{
		s_data->sceneWideUniforms = sceneWideUniforms;

	}
	void Renderer3D::submit(const std::shared_ptr<VertexArray>& geometry, const std::shared_ptr<Material>& material, const glm::mat4& model)
	{
		//Bind a shader
		glUseProgram(material->getShader()->getID());

		//Apply sceneWideUniforms
		for (auto& dataPair : s_data->sceneWideUniforms)
		{
			const char* nameOfUniform = dataPair.first;
			ShaderDataType& sdt = dataPair.second.first;
			void* addressOfValue = dataPair.second.second;

			switch (sdt)
			{
			case ShaderDataType::Int:
				material->getShader()->uploadInt(nameOfUniform, *(int*)addressOfValue);
				break;
			case ShaderDataType::Float3:
				material->getShader()->uploadFloat3(nameOfUniform,*(glm::vec3*)addressOfValue);
				break;
			case ShaderDataType::Float4:
				material->getShader()->uploadFloat4(nameOfUniform, *(glm::vec4*)addressOfValue);
				break;
			case ShaderDataType::Mat4:
				material->getShader()->uploadMat4(nameOfUniform, *(glm::mat4*)addressOfValue);
				break;
			}
		}

		//Apply Material uniforms (per draw uniforms)
		material->getShader()->uploadMat4("u_model", model);

		std::shared_ptr<Texture> texture;
		if (material->isFlagSet(Material::flag_texture)) texture = material->getTexture();
		else texture = s_data->defaultTexture;

		uint32_t texUnit;
		if (RendererCommon::texUnitMan.getUnit(texture->getID(), texUnit))
		{
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_2D, texture->getID());
		}
		material->getShader()->uploadInt("u_texData", texUnit);




		if (material->isFlagSet(Material::flag_tint))
			material->getShader()->uploadFloat4("u_tint", material->getTint());
		else
			material->getShader()->uploadFloat4("u_tint", s_data->defaultTint);

		//Bind the geometry (VAO and IBO)
		glBindVertexArray(geometry->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->getIndexBuffer()->getID());

		//Submit the draw call 
		glDrawElements(GL_TRIANGLES, geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer3D::end()
	{
		s_data->sceneWideUniforms.clear();
	}
}