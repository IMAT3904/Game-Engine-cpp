#pragma once
/* \file Renderer3D.h */

#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>
#include "rendering/texture.h"
#include "rendering/shader.h"
#include "rendering/RenderAPI.h"
#include "rendering/shaderDataType.h"
#include "vertexArray.h"

namespace Engine
{
	using SceneWideUniforms = std::unordered_map<const char*, std::pair<ShaderDataType, void*>>;

	class Material;
	/** \class Renderer3D
	** \brief A class which renders 3D geometry instantly (not batched)
	*/
	class Renderer3D
	{
	public:
		static void init(); //!< Init the renderer
		static void begin(const SceneWideUniforms& sceneWideUniforms); //!< Begin a new 3d scene
		static void submit(const std::shared_ptr<VertexArray>& geometry, const std::shared_ptr<Material>& material, const glm::mat4& model); //!< submit a piece of geometry to be rendered
		static void end(); //!< end the current 3D scene
	private:
		struct InternalData
		{
			SceneWideUniforms sceneWideUniforms; //!< Replace with UBO at some point
			std::shared_ptr<Texture> defaultTexture; //!< Empty white texture
			glm::vec4 defaultTint //!< Default white tint
		};
		static std::shared_ptr<InternalData> s_data; //!< Data internal to the renderer 
	};
}