/** \file renderer3d.cpp*/

#include "engine_pch.h"
#include "rendering/Renderer3D.h"

namespace Engine
{
	std::shared_ptr<Renderer3D::InternalData> Renderer3D::s_data = nullptr;
	void Renderer3D::init()
	{
		s_data.reset(new InternalData);

		unsigned char whitePX[4] = { 255,255,255,255 };
		s_data.reset(Texture::create(1, 1, 4, whitePX));
		s_data->defaultTint = { 1.f,1.f,1.f,1.f };

	}
	void Renderer3D::begin(const SceneWideUniforms& sceneWideUniforms)
	{
		s_data->sceneWideUniforms = sceneWideUniforms;

	}
	void Renderer3D::submit(const std::shared_ptr<VertexArray>& geometry, const std::shared_ptr<Material>& material, const glm::mat4& model)
	{

	}
	void Renderer3D::end()
	{
		s_data->sceneWideUniforms.clear();
	}
}