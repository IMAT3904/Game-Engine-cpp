#pragma once
/* \file renderer2D.h */

#include <glm/glm.hpp>
#include "rendering/RendererCommon.h"

namespace Engine
{
	class Quad;
	/*
	** \class Renderer 2D
	** \brief Class which allows the rendering of simple 2D primitives
	*/

	class Renderer2D
	{
	public:
		static void init(); //!< Init the internal data of the renderer
		static void begin(const SceneWideUniforms& swu); //!< Begin a 2D scene
		static void submit(const Quad& quad, const glm::vec4& tint); //!< Render a tinted quad;
		static void end();
	private:
		struct InternalData
		{
			std::shared_ptr<Texture> defaultTexture;
			glm::vec4 defaultTint;
			std::shared_ptr<Shader> shader;
			std::shared_ptr<VertexArray> VAO;
			glm::mat4 model;
		};

		static std::shared_ptr<InternalData> s_data;
	};


}
