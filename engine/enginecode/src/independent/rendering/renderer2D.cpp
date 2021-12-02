/* file renderer2D.cpp*/

#include "engine_pch.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "rendering/Renderer2D.h"
#include "systems/log.h"


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
	void Renderer2D::submit(const Quad& quad, const glm::vec4 & tint)
	{
		Renderer2D::submit(quad, tint, s_data->defaultTexture);
	}
	void Renderer2D::submit(const Quad& quad, const std::shared_ptr<Texture>& texture)
	{
		Renderer2D::submit(quad, s_data->defaultTint, texture);
	}
	void Renderer2D::submit(const Quad& quad, const SubTexture& subtexture)
	{
		Renderer2D::submit(quad, s_data->defaultTint, subtexture);
	}
	void Renderer2D::submit(const Quad& quad, const std::shared_ptr<Texture>& texture, float angle, bool degrees)
	{
		Renderer2D::submit(quad, s_data->defaultTint, texture,angle,degrees);
	}
	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, const std::shared_ptr<Texture>& texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture->getID());
		s_data->model = glm::scale(glm::translate(glm::mat4(1.f), quad.m_translate), quad.m_scale);

		s_data->shader->uploadInt("u_texData", 0);
		s_data->shader->uploadFloat4("u_tint", tint);
		s_data->shader->uploadMat4("u_model", s_data->model);

		glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, const SubTexture& subtexture)
	{
		glBindTexture(GL_TEXTURE_2D, subtexture.getBaseTexture()->getID());
		s_data->model = glm::scale(glm::translate(glm::mat4(1.f), quad.m_translate), quad.m_scale);

		s_data->shader->uploadInt("u_texData", 0);
		s_data->shader->uploadFloat4("u_tint", tint);
		s_data->shader->uploadMat4("u_model", s_data->model);

		glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, float angle, bool degrees)
	{
		Renderer2D::submit(quad, tint, s_data->defaultTexture,angle,degrees);
	}
	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, const std::shared_ptr<Texture>& texture, float angle, bool degrees)
	{
		if (degrees) angle = glm::radians(angle);

		glBindTexture(GL_TEXTURE_2D, texture->getID());
		s_data->model = glm::scale(glm::rotate(glm::translate(glm::mat4(1.f), quad.m_translate), angle, {0.f,0.f,1.f}), quad.m_scale);

		s_data->shader->uploadInt("u_texData", 0);
		s_data->shader->uploadFloat4("u_tint", tint);
		s_data->shader->uploadMat4("u_model", s_data->model);

		glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

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

	Quad Quad::createCentreHalfExtents(const glm::vec2& centre, const glm::vec2& halfExtents)
	{
		Quad result;
		result.m_translate = glm::vec3(centre, 0.f);
		result.m_scale = glm::vec3(halfExtents * 2.f, 1.f);
		
		return result;
	}
}