#pragma once
/* \file renderer2D.h */

#include <glm/glm.hpp>
#include "rendering/RendererCommon.h"
#include "rendering/uniformBuffer.h"

#include <ft2build.h>
#include "freetype/freetype.h"
#include "rendering/subTexture.h"
#include <unordered_map>
#include <array>
#include <numeric>
namespace Engine
{
	class Renderer2DVertex
	{
	public:
		Renderer2DVertex() = default;
		Renderer2DVertex(const glm::vec4& pos, const glm::vec2& UVs, uint32_t tu, const glm::vec4& pTint) :
			position(pos), uvCoords(UVs), texUnit(tu), tint(pack(pTint)) {};
		Renderer2DVertex(const glm::vec4& pos, const glm::vec2& UVs, uint32_t tu) : position(pos), uvCoords(UVs), texUnit(tu) {};

		glm::vec4 position;
		glm::vec2 uvCoords;
		uint32_t texUnit;
		uint32_t tint;
		static VertexBufferLayout layout;
		static uint32_t pack(const glm::vec4& tint);
	};
	
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

	struct CharacterData
	{
		//The glyph data

		glm::vec2 glyphSize;
		glm::vec2 glyphBearing;
		float advance;
		SubTexture subTexture;
	};

	class Renderer2D
	{
	public:
		static void init(); //!< Init the internal data of the renderer
		static void begin(const SceneWideUniforms& swu); //!< Begin a 2D scene
		static void submit(const Quad& quad, const glm::vec4& tint); //!< Render a tinted quad;
		static void submit(const Quad& quad, const SubTexture& subtexture); //!< Render a textured quad with subtexture;
		static void submit(const Quad& quad, const glm::vec4& tint  ,const SubTexture& subtexture ); //!< Render a textured quad with subtexture and tint;
		static void submit(const Quad& quad, const SubTexture& texture, float angle, bool degrees = false); //!< Render a textured quad;
		//static void submit(const Quad& quad, const glm::vec4& tint, const SubTexture& texture); //!< Render a textured and tinted quad;
		static void submit(const Quad& quad, const glm::vec4& tint, float angle, bool degrees = false); //!< Render a textured and tinted quad;
		static void submit(const Quad& quad, const glm::vec4& tint, const SubTexture& texture,float angle, bool degrees = false); //!< Render a textured and tinted quad;
		
		static void submit(char ch, const glm::vec2& position, float& advance, const glm::vec4 tint); //!< Render a single character with a tint
		static void flush();
		static void end();
	private:
		struct InternalData
		{
			std::shared_ptr<Texture> defaultTexture;
			SubTexture defaultSubTexture;
			glm::vec4 defaultTint;
			std::shared_ptr<Shader> shader;
			std::shared_ptr<VertexArray> VAO;
			std::shared_ptr<UniformBuffer> UBO;
			std::vector<Renderer2DVertex> vertices;
			std::array<glm::vec4, 4> quads;
			std::array<int32_t, 32> textureUnits;
			glm::mat4 model;
			FT_Library ft;
			FT_Face fontFace;
			std::shared_ptr<Texture> fontTexture;
			glm::ivec2 glyphBUfferDims;
			uint32_t glyphBufferSize;
			std::shared_ptr<unsigned char> glyphBuffer;
			unsigned char first_char;
			unsigned char last_char;
			std::unordered_map<unsigned char, CharacterData> charactersData;
			static const uint32_t batchSize = 8192;
			uint32_t drawCount;
		};

		static std::shared_ptr<InternalData> s_data;

		static unsigned char* rtoRGBA(unsigned char* rBuffer, uint32_t width, uint32_t height);
	};

	

}
