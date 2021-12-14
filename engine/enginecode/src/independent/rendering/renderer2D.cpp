/* file renderer2D.cpp*/

#include "engine_pch.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "rendering/Renderer2D.h"
#include "systems/log.h"


namespace Engine
{

	VertexBufferLayout Renderer2DVertex::layout = VertexBufferLayout({ ShaderDataType::Float4, ShaderDataType::Float2,ShaderDataType::FlatInt, {ShaderDataType::Byte4,true} });
	std::shared_ptr<Renderer2D::InternalData> Renderer2D::s_data = nullptr;

	void Renderer2D::init()
	{
		int i = sizeof(Renderer2DVertex);

		s_data.reset(new InternalData);

		s_data->textureUnits = { 0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };


		unsigned char whitePX[4] = { 255,255,255,255 };
		s_data->defaultTexture.reset(Texture::create(1, 1, 4, whitePX));
		s_data->defaultSubTexture = SubTexture(s_data->defaultTexture, { 0,0 }, {1, 1});

		s_data->defaultTint = { 1.f,1.f,1.f,1.f };
		s_data->model = glm::mat4(1.0f);

		s_data->shader.reset(Shader::create("./assets/shaders/quad2.glsl"));
		glUseProgram( s_data->shader->getID());
		s_data->UBO.reset(UniformBuffer::create(UniformBufferLayout({
			{ "u_projection",ShaderDataType::Mat4 },
			{ "u_view",ShaderDataType::Mat4 }
	})));


		s_data->shader->uploadIntArray("u_texData", s_data->textureUnits.data(), 32);

		s_data->quads[0] = { -0.5f, -0.5f,1,1 };
		s_data->quads[1] = { -0.5f,  0.5f,1,1 };
		s_data->quads[2] = { 0.5f,  0.5f,1,1 };
		s_data->quads[3] = { 0.5f, -0.5f,1,1 };

		
		s_data->vertices.resize(s_data->batchSize);
		s_data->vertices[0] = Renderer2DVertex(s_data->quads[0],{ 0.f, 0.f  },0, glm::vec4(1.f));
		s_data->vertices[1] = Renderer2DVertex(s_data->quads[1], { 0.f, 1.f },0, glm::vec4(1.f));
		s_data->vertices[2] = Renderer2DVertex(s_data->quads[2], { 1.f, 1.f },0, glm::vec4(1.f));
		s_data->vertices[3] = Renderer2DVertex(s_data->quads[3], { 1.f, 0.f },0, glm::vec4(1.f));



		std::vector<uint32_t> indices(s_data->batchSize);
		std::iota(indices.begin(), indices.end(), 0);

		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<IndexBuffer> IBO;

		s_data->VAO.reset(VertexArray::create());
		VBO.reset(VertexBuffer::create(s_data->vertices.data(), sizeof(Renderer2DVertex) * s_data->vertices.size(), Renderer2DVertex::layout));
		IBO.reset(IndexBuffer::create(indices.data(), indices.size()));
		s_data->VAO->addVertexBuffer(VBO);
		s_data->VAO->setIndexBuffer(IBO);

		s_data->UBO->attachShaderBlock(s_data->shader, "b_camera");

		//File path to a font
		const char* filePath = "./assets/fonts/arial.ttf";

		//Set the dim of the glyph buffer
		s_data->glyphBUfferDims = { 256,256 };
		s_data->glyphBufferSize = s_data->glyphBUfferDims.x * s_data->glyphBUfferDims.y * 4 * sizeof(unsigned char);
		s_data->glyphBuffer.reset(static_cast<unsigned char *> (malloc(s_data->glyphBufferSize)));


		//Initialize freetype
		if (FT_Init_FreeType(&s_data->ft)) Log::error("Error: Freetype could not be initialised");

		//Load the font 
		if (FT_New_Face(s_data->ft, filePath, 0, &s_data->fontFace)) Log::error("Error: Freetype could not load font: {0}", filePath);

		//Set the char size 
		int32_t charSize = 86;
		if (FT_Set_Pixel_Sizes(s_data->fontFace, 0, charSize)) Log::error("Error: freetype can't set font size: {0}", charSize);

		s_data->first_char = ' ';
		s_data->last_char = '~';

		//Do a big texture for all characters

		int32_t charCount = s_data->last_char - s_data->first_char;

		uint32_t maxCharWidth = 0, maxCharHeight = 0;


		for (unsigned char c = s_data->first_char; c <= s_data->last_char; c++)
		{
			//Get glyph from freetype
			if (FT_Load_Char(s_data->fontFace, c, FT_LOAD_RENDER)) Log::error("Error: Could not load glyph for char{0}", c);
			else
			{
				//Get glyph data
				uint32_t glyphWidth = s_data->fontFace->glyph->bitmap.width;
				uint32_t glyphHeight = s_data->fontFace->glyph->bitmap.rows;

				maxCharWidth = std::max(maxCharWidth, glyphWidth);
				maxCharHeight = std::max(maxCharHeight, glyphHeight);
			}
		}

		maxCharWidth += 10;
		maxCharHeight += 10;
		uint32_t textureWidth =maxCharWidth*10, textureHeight=maxCharHeight*10;


		unsigned char* texData = static_cast<unsigned char*>(malloc(textureWidth * textureHeight * 4 * sizeof(unsigned char)));

		memset(texData, 0, textureWidth * textureHeight * 4);

		//Init font texture
		s_data->fontTexture.reset(Texture::create(textureWidth, textureHeight, 4, texData));

		free(texData);

		for (unsigned char c = s_data->first_char; c <= s_data->last_char; c++)
		{
			//Get glyph from freetype
			if (FT_Load_Char(s_data->fontFace, c, FT_LOAD_RENDER)) Log::error("Error: Could not load glyph for char{0}", c);
			else
			{
				CharacterData cData;
				//Get glyph data
				uint32_t glyphWidth = s_data->fontFace->glyph->bitmap.width;
				uint32_t glyphHeight = s_data->fontFace->glyph->bitmap.rows;

				cData.glyphSize = glm::vec2(glyphWidth, glyphHeight);
				cData.glyphBearing = glm::vec2(s_data->fontFace->glyph->bitmap_left, -s_data->fontFace->glyph->bitmap_top);


				//Calculate the advance
				cData.advance = static_cast<float>(s_data->fontFace->glyph->advance.x >> 6);

				//Calculate the quad for the glyph
				/*glm::vec2 glyphHalfExtents = glm::vec2(s_data->fontTexture->getWidthf() * 0.5f, s_data->fontTexture->getHeight() * 0.5f);
				glm::vec2 glyphCentre = (position + glyphBearing) + glyphHalfExtents;

				Quad quad = Quad::createCentreHalfExtents(glyphCentre, glyphHalfExtents);*/

				uint32_t texPosX = (c - s_data->first_char) / 10;
				uint32_t texPosY = (c - s_data->first_char) % 10;
				texPosX *= maxCharWidth;
				texPosY *= maxCharHeight;

				unsigned char* glyphRGBABuffer = rtoRGBA(s_data->fontFace->glyph->bitmap.buffer, glyphWidth, glyphHeight);
				s_data->fontTexture->edit(texPosX, texPosY, glyphWidth, glyphHeight, glyphRGBABuffer);
				free(glyphRGBABuffer);

				glm::vec2 uvStart(texPosX / textureWidth, texPosY /textureHeight);
				glm::vec2 uvEnd((texPosX +glyphWidth) / textureWidth, (texPosY +glyphHeight) /textureHeight);

				cData.subTexture = SubTexture(s_data->fontTexture, uvStart, uvEnd);

				s_data->charactersData[c] = cData;
				//Submit quad
				//submit(quad, tint, s_data->fontTexture);
			}
		}

		

		// Send glyph buffer to the texture on the GPU
		//s_data->fontTexture->edit(0, 0, s_data->glyphBUfferDims.x, s_data->glyphBUfferDims.y, s_data->glyphBuffer.get());

	}
	void Renderer2D::begin(const SceneWideUniforms& swu)
	{
		//Bind the shader
		glUseProgram(s_data->shader->getID());

		//Reset drawcount
		s_data->drawCount = 0;

		glBindBuffer(GL_UNIFORM_BUFFER, s_data->UBO->GetRenderID());
		s_data->UBO->uploadData("u_projection", swu.at("u_projection").second);
		s_data->UBO->uploadData("u_view", swu.at("u_view").second);


		//Bind the geometry
		//glBindVertexArray(s_data->VAO->getID());
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->VAO->getIndexBuffer()->getID());
	}
	void Renderer2D::submit(const Quad& quad, const glm::vec4 & tint)
	{
		Renderer2D::submit(quad, tint,s_data->defaultSubTexture);
	}
	void Renderer2D::submit(const Quad& quad, const SubTexture& texture)
	{
		Renderer2D::submit(quad, s_data->defaultTint, texture);
	}
	void Renderer2D::submit(const Quad& quad, const SubTexture& texture, float angle, bool degrees)
	{
		Renderer2D::submit(quad, s_data->defaultTint, texture,angle,degrees);
	}
	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, const SubTexture& texture)
	{
		if (s_data->drawCount + 4 > s_data->batchSize) flush();

		uint32_t texUnit;
		if (RendererCommon::texUnitMan.full()) flush();
		if (RendererCommon::texUnitMan.getUnit(texture.getBaseTexture()->getID(), texUnit))
		{
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_2D, texture.getBaseTexture()->getID());
		}


		
		s_data->model = glm::scale(glm::translate(glm::mat4(1.f), quad.m_translate), quad.m_scale);
		uint32_t packedTint = Renderer2DVertex::pack(tint);


		//s_data->shader->uploadInt("u_texData", 0);
		//s_data->shader->uploadFloat4("u_tint", tint);

		uint32_t startIdx = s_data->drawCount;
		for (uint32_t i = 0; i < 4; i++)
		{
			s_data->vertices[i + startIdx].position = s_data->model * s_data->quads[i];
			s_data->vertices[i + startIdx].tint = packedTint;
			s_data->vertices[i + startIdx].texUnit = texUnit;
		}

		s_data->vertices[startIdx + 0].uvCoords = texture.getUVStart();
		s_data->vertices[startIdx + 1].uvCoords = { texture.getUVStart().x,texture.getUVEnd().y };
		s_data->vertices[startIdx + 2].uvCoords = texture.getUVEnd();
		s_data->vertices[startIdx + 3].uvCoords = { texture.getUVEnd().x,texture.getUVStart().y };

		s_data->drawCount += 4;

		//->VAO->GetVertexBuffers().at(0)->edit(s_data->vertices.data(), sizeof(Renderer2DVertex) * s_data->vertices.size(), 0);

		//glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

	}

	//void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, const SubTexture& subtexture)
	//{
	//	glBindTexture(GL_TEXTURE_2D, subtexture.getBaseTexture()->getID());
	//	s_data->model = glm::scale(glm::translate(glm::mat4(1.f), quad.m_translate), quad.m_scale);

	//	s_data->shader->uploadInt("u_texData", 0);
	//	s_data->shader->uploadFloat4("u_tint", tint);

	//	glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	//}
	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, float angle, bool degrees)
	{
		Renderer2D::submit(quad, tint, s_data->defaultSubTexture,angle,degrees);
	}
	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, const SubTexture& texture, float angle, bool degrees)
	{
		if (degrees) angle = glm::radians(angle);

		if (s_data->drawCount + 4 > s_data->batchSize) flush();

		uint32_t texUnit;
		if (RendererCommon::texUnitMan.full()) flush();
		if (RendererCommon::texUnitMan.getUnit(texture.getBaseTexture()->getID(), texUnit))
		{
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_2D, texture.getBaseTexture()->getID());
		}

		s_data->model = glm::scale(glm::rotate(glm::translate(glm::mat4(1.f), quad.m_translate), angle, {0.f,0.f,1.f}), quad.m_scale);

		s_data->shader->uploadInt("u_texData", 0);
		s_data->shader->uploadFloat4("u_tint", tint);
		uint32_t packedTint = Renderer2DVertex::pack(tint);

		uint32_t startIdx = s_data->drawCount;
		for (uint32_t i = 0; i < 4; i++)
		{
			s_data->vertices[i + startIdx].position = s_data->model * s_data->quads[i];
			s_data->vertices[i + startIdx].tint = packedTint;
			s_data->vertices[i + startIdx].texUnit = texUnit;
		}

		s_data->vertices[startIdx + 0].uvCoords = texture.getUVStart();
		s_data->vertices[startIdx + 1].uvCoords = { texture.getUVStart().x,texture.getUVEnd().y };
		s_data->vertices[startIdx + 2].uvCoords = texture.getUVEnd();
		s_data->vertices[startIdx + 3].uvCoords = { texture.getUVEnd().x,texture.getUVStart().y };

		s_data->drawCount += 4;

		//s_data->VAO->GetVertexBuffers().at(0)->edit(s_data->vertices.data(), sizeof(Renderer2DVertex) * s_data->vertices.size(), 0);

		//glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

	}
	void Renderer2D::submit(char ch, const glm::vec2& position, float& advance, const glm::vec4 tint)
	{
		////Get glyph from freetype
		//if (FT_Load_Char(s_data->fontFace, ch, FT_LOAD_RENDER)) Log::error("Error: Could not load glyph for char{0}", ch);
		//else
		//{
		//	//Get glyph data
		//	uint32_t glyphWidth = s_data->fontFace->glyph->bitmap.width;
		//	uint32_t glyphHeight = s_data->fontFace->glyph->bitmap.rows;
		//	glm::vec2 glyphSize(glyphWidth, glyphHeight);
		//	glm::vec2 glyphBearing(s_data->fontFace->glyph->bitmap_left, -s_data->fontFace->glyph->bitmap_top);

		//	//Calculate the advance
		//	advance = static_cast<float>(s_data->fontFace->glyph->advance.x >> 6);

		//	//Calculate the quad for the glyph
		//	glm::vec2 glyphHalfExtents = glm::vec2(s_data->fontTexture->getWidthf() * 0.5f, s_data->fontTexture->getHeight() * 0.5f);
		//	glm::vec2 glyphCentre = (position + glyphBearing) + glyphHalfExtents;

		//	Quad quad = Quad::createCentreHalfExtents(glyphCentre, glyphHalfExtents);

		//	unsigned char* glyphRGBABuffer = rtoRGBA(s_data->fontFace->glyph->bitmap.buffer, glyphWidth, glyphHeight);
		//	s_data->fontTexture->edit(0, 0, glyphWidth, glyphHeight, glyphRGBABuffer);
		//	free(glyphRGBABuffer);

		//	//Submit quad
		//	submit(quad, tint, s_data->fontTexture);
		//}
		//Quad quad = Quad::createCentreHalfExtents({ 512,400 }, { 400,400 });
		//submit(quad, glm::vec4(1), s_data->fontTexture);
		
		auto& cData = s_data->charactersData[ch];

		glm::vec2 centre = position + cData.glyphBearing + cData.glyphSize * 0.5f;
		glm::vec2 he = cData.glyphSize * 0.5f;
		
		Quad quad = Quad::createCentreHalfExtents(centre, he);


		submit(quad, tint, cData.subTexture);
	}
	void Renderer2D::end()
	{
		if (s_data->drawCount > 0) flush();
	}

	unsigned char* Renderer2D::rtoRGBA(unsigned char* rBuffer, uint32_t width, uint32_t height)
	{
		uint32_t rBufferSize = width * height * 4 * sizeof(unsigned char);
		unsigned char* result = (unsigned char*)malloc(rBufferSize);
		memset(result, 255, rBufferSize);

		unsigned char* pWalker = result;
		for (int32_t i = 0; i < height; i++)
		{
			for (int32_t j = 0; j < width; j++)
			{
				pWalker+=3; // Go to A
				*pWalker = *rBuffer; //Set alpha channel
				pWalker++;
				rBuffer++;
			}
		}
		return result;
	}

	void Renderer2D::flush()
	{
		s_data->VAO->GetVertexBuffers().at(0)->edit(s_data->vertices.data(), sizeof(Renderer2DVertex) * s_data->drawCount, 0);

		glBindVertexArray(s_data->VAO->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->VAO->getIndexBuffer()->getID());

		glDrawElements(GL_QUADS, s_data->drawCount, GL_UNSIGNED_INT, nullptr);
		s_data->drawCount = 0;
	}

	Quad Quad::createCentreHalfExtents(const glm::vec2& centre, const glm::vec2& halfExtents)
	{
		Quad result;
		result.m_translate = glm::vec3(centre, 0.f);
		result.m_scale = glm::vec3(halfExtents * 2.f, 1.f);
		
		return result;
	}
	uint32_t Renderer2DVertex::pack(const glm::vec4& tint)
	{
		uint32_t result = 0;
		uint32_t r = (static_cast<uint32_t>(tint.r * 255.0f)) << 0; //000R
		uint32_t g = (static_cast<uint32_t>(tint.g * 255.0f)) << 8; //00B0
		uint32_t b = (static_cast<uint32_t>(tint.b * 255.0f)) << 16;//0G00
		uint32_t a = (static_cast<uint32_t>(tint.a * 255.0f)) << 24;//A

		result = (r | g | b | a);
		return result;
	}
}