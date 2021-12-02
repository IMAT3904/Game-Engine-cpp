#pragma once
/* \file renderer2D.h */

#include <glm/glm.hpp>
#include "rendering/RendererCommon.h"

#include <ft2build.h>
#include "freetype/freetype.h"

namespace Engine
{
	class Quad
	{
	public:
		Quad() = default;
		static Quad createCentreHalfExtents(const glm::vec2& centre, const glm::vec2& halfExtents);
	private:
		glm::vec3 m_translate = glm::vec3(0.f); //!< Translation vector
		glm::vec3 m_scale = glm::vec3(1.f); //!< Scale vector
		friend class Renderer2D;
	};
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
		static void submit(const Quad& quad, const std::shared_ptr<Texture>& texture); //!< Render a textured quad;
		static void submit(const Quad& quad, const std::shared_ptr<Texture>& texture, float angle, bool degrees = false); //!< Render a textured quad;
		static void submit(const Quad& quad, const glm::vec4& tint, const std::shared_ptr<Texture>& texture); //!< Render a textured and tinted quad;
		static void submit(const Quad& quad, const glm::vec4& tint, float angle, bool degrees = false); //!< Render a textured and tinted quad;
		static void submit(const Quad& quad, const glm::vec4& tint, const std::shared_ptr<Texture>& texture,float angle, bool degrees = false); //!< Render a textured and tinted quad;
		
		static void submit(char ch, const glm::vec2& position, float& advance, const glm::vec4 tint); //!< Render a single character with a tint
		
		static void end();
	private:
		struct InternalData
		{
			std::shared_ptr<Texture> defaultTexture;
			glm::vec4 defaultTint;
			std::shared_ptr<Shader> shader;
			std::shared_ptr<VertexArray> VAO;
			glm::mat4 model;
			FT_Library ft;
			FT_Face fontFace;
			std::shared_ptr<Texture> fontTexture;
			glm::ivec2 glyphBUfferDims;
			uint32_t glyphBufferSize;
			std::shared_ptr<unsigned char> glyphBuffer;
		};

		static std::shared_ptr<InternalData> s_data;

		static unsigned char* rtoRGBA(unsigned char* rBuffer, uint32_t width, uint32_t height);
	};


}
