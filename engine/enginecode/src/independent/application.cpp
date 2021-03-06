/** \file application.cpp
*/

#include "engine_pch.h"
#include <glad/glad.h>
#include "core/application.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWSystem.h"
#endif

#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rendering/indexBuffer.h"
#include "rendering/shader.h"
#include "rendering/texture.h"
#include "rendering/vertexBuffer.h"
#include "rendering/vertexArray.h"
#include "rendering/subTexture.h"
#include "rendering/Renderer3D.h"
#include "rendering/Renderer2D.h"
#include "rendering/uniformBuffer.h"
#include "rendering/camera.h"

namespace Engine {
	// Set static vars
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		//Start log
		m_logSystem.reset(new Log);
		m_logSystem->start();

		//Start Timer
		m_timer.reset(new Timer);
		m_timer->start();

		//Start windows system
#ifdef NG_PLATFORM_WINDOWS
		m_windowsSystem.reset(new GLFWSystem);
#endif // NG_PLATFORM_WINDOWS
		m_windowsSystem->start();

		WindowProperties props("My game window", 1024, 800, 0, 0);
		m_window.reset(Window::create(props));


		m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
		InputPoller::setNativeWindow(m_window->getNativeWindow());

		//Little experiment
		// m_timer->start();
		//for (int i = 0; i < 100; i += 1)
		//{
		//	Log::info(i);
		//}
		//Log::info(" took exactly {0} ",m_timer->getElapsedTime());
	}

	Application::~Application()
	{
		//Stop windows
		m_windowsSystem->stop();

		//Stop logging
		m_logSystem->stop();
	}


#pragma region TEMP_CLASS
	class TPVertex
	{
	public:
		glm::vec3 m_pos;
		glm::vec3 m_normal;
		glm::vec2 m_uv;
		TPVertex() : m_pos(glm::vec3(0.f)), m_uv(glm::vec2(0.f)), m_normal(glm::vec3(0)) {};
		TPVertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv) :m_pos(pos), m_normal(norm), m_uv(uv) {}
		static VertexBufferLayout GetLayout() { return s_layout; }
	private:
		static VertexBufferLayout s_layout;
	};

	VertexBufferLayout TPVertex::s_layout = { ShaderDataType::Float3, {ShaderDataType::Short3}, {ShaderDataType::Short3} };

	class TPVertexNormalised
	{
	public:
		glm::vec3 m_pos;
		std::array<int16_t, 3> m_normal;
		std::array<int16_t, 2> m_uv;
		TPVertexNormalised() : m_pos(glm::vec3(0.f)), m_uv({ 0,0 }), m_normal({ 0,0,0 }) {};
		TPVertexNormalised(const glm::vec3& pos, const std::array<int16_t, 3>& norm, const std::array<int16_t, 2>& uv) :m_pos(pos), m_normal(norm), m_uv(uv) {}
		static inline VertexBufferLayout GetLayout() { return s_layout; }
	private:
		static VertexBufferLayout s_layout;
	};
	VertexBufferLayout TPVertexNormalised::s_layout = { { ShaderDataType::Float3, {ShaderDataType::Short3, true}, {ShaderDataType::Short3, true}} , 24 };

	class TPVertexNormalisedTint
	{
	public:
		glm::vec3 m_pos;
		std::array<int16_t, 2> m_uv;
		uint32_t m_tint;
		std::array<int16_t, 3> m_normal;
		TPVertexNormalisedTint() : m_pos(glm::vec3(0.f)), m_uv({ 0,0 }), m_normal({ 0,0,0 }), m_tint(0) {};
		TPVertexNormalisedTint(const glm::vec3& pos, const std::array<int16_t, 3>& norm, const std::array<int16_t, 2>& uv, uint32_t tint) :m_pos(pos), m_normal(norm), m_uv(uv), m_tint(tint) {}
		static inline VertexBufferLayout GetLayout() { return s_layout; }
	private:
		static VertexBufferLayout s_layout;
	};
	VertexBufferLayout TPVertexNormalisedTint::s_layout = { { ShaderDataType::Float3, {ShaderDataType::Short3, true}, { ShaderDataType::Byte4, true }, {ShaderDataType::Short3, true}},28 };



	std::array<int16_t, 3> normalise(const glm::vec3& normal)
	{
		std::array<int16_t, 3> result;

		if (normal.x == 1.0) result.at(0) = INT16_MAX;
		else if (normal.x == -1.0) result.at(0) = INT16_MIN;
		else result.at(0) = static_cast<int16_t>(normal.x * static_cast<float>(INT16_MAX));


		if (normal.y == 1.0) result.at(1) = INT16_MAX;
		else if (normal.y == -1.0) result.at(1) = INT16_MIN;
		else result.at(1) = static_cast<int16_t>(normal.y * static_cast<float>(INT16_MAX));


		if (normal.z == 1.0) result.at(2) = INT16_MAX;
		else if (normal.z == -1.0) result.at(2) = INT16_MIN;
		else result.at(2) = static_cast<int16_t>(normal.z * static_cast<float>(INT16_MAX));

		return result;
		
	}

	std::array<int16_t, 2> normalise(const glm::vec2& uv)
	{
		std::array<int16_t, 2> result;

		if (uv.x == 1.0) result.at(0) = INT16_MAX;
		else if (uv.x == -1.0) result.at(0) = INT16_MIN;
		else result.at(0) = static_cast<int16_t>(uv.x * static_cast<float>(INT16_MAX));


		if (uv.y == 1.0) result.at(1) = INT16_MAX;
		else if (uv.y == -1.0) result.at(1) = INT16_MIN;
		else result.at(1) = static_cast<int16_t>(uv.y * static_cast<float>(INT16_MAX));

		return result;
	}

	uint32_t pack(const glm::vec3& colour)
	{
		return pack({ colour.x, colour.y, colour.z, 1.f });
	}

#pragma endregion

	void Application::run()
	{
#pragma region RAW_DATA

		std::vector<TPVertexNormalised> cubeVertices(24);

		cubeVertices.at(0) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f},normalise({0.f,  0.f, -1.f}),normalise({0.f,   0.f }));
		cubeVertices.at(1) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f},normalise({0.f,  0.f, -1.f}),normalise({0.f,   0.5f}));
		cubeVertices.at(2) = TPVertexNormalised({-0.5f, -0.5f, -0.5f},normalise({0.f,  0.f, -1.f}),normalise({0.33f, 0.5f}));
		cubeVertices.at(3) = TPVertexNormalised({-0.5f,  0.5f, -0.5f},normalise({0.f,  0.f, -1.f}),normalise({0.33f, 0.f }));
		cubeVertices.at(4) = TPVertexNormalised({-0.5f, -0.5f, 0.5f },normalise({0.f,  0.f,  1.f}),normalise({0.33f, 0.5f}));
		cubeVertices.at(5) = TPVertexNormalised({ 0.5f, -0.5f, 0.5f },normalise({0.f,  0.f,  1.f}),normalise({0.66f, 0.5f}));
		cubeVertices.at(6) = TPVertexNormalised({ 0.5f,  0.5f, 0.5f },normalise({0.f,  0.f,  1.f}),normalise({0.66f, 0.f }));
		cubeVertices.at(7) = TPVertexNormalised({-0.5f,  0.5f, 0.5f },normalise({0.f,  0.f,  1.f}),normalise({0.33,  0.f }));
		cubeVertices.at(8) = TPVertexNormalised({-0.5f, -0.5f, -0.5f},normalise({0.f, -1.f,  0.f}),normalise({1.f,   0.f }));
		cubeVertices.at(9) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f},normalise({0.f, -1.f,  0.f}),normalise({0.66f, 0.f }));
		cubeVertices.at(10) = TPVertexNormalised({ 0.5f, -0.5f, 0.5f },normalise({0.f, -1.f,  0.f}),normalise({0.66f, 0.5f}));
		cubeVertices.at(11) = TPVertexNormalised({-0.5f, -0.5f, 0.5f },normalise({0.f, -1.f,  0.f}),normalise({1.0f,  0.5f}));
		cubeVertices.at(12) = TPVertexNormalised({ 0.5f,  0.5f, 0.5f },normalise({0.f,  1.f,  0.f}),normalise({0.f,   0.5f}));
		cubeVertices.at(13) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f},normalise({0.f,  1.f,  0.f}),normalise({0.f,   1.0f}));
		cubeVertices.at(14) = TPVertexNormalised({-0.5f,  0.5f, -0.5f},normalise({0.f,  1.f,  0.f}),normalise({0.33f, 1.0f}));
		cubeVertices.at(15) = TPVertexNormalised({-0.5f,  0.5f, 0.5f },normalise({0.f,  1.f,  0.f}),normalise({0.3f,  0.5f}));
		cubeVertices.at(16) = TPVertexNormalised({-0.5f,  0.5f, 0.5f },normalise({-1.f,  0.f,  0.f}),normalise({0.66f, 0.5f}));
		cubeVertices.at(17) = TPVertexNormalised({-0.5f,  0.5f, -0.5f},normalise({-1.f,  0.f,  0.f}),normalise({0.33f, 0.5f}));
		cubeVertices.at(18) = TPVertexNormalised({-0.5f, -0.5f, -0.5f},normalise({-1.f,  0.f,  0.f}),normalise({0.33f, 1.0f}));
		cubeVertices.at(19) = TPVertexNormalised({-0.5f, -0.5f, 0.5f },normalise({-1.f,  0.f,  0.f}),normalise({0.66f, 1.0f}));
		cubeVertices.at(20) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f},normalise({1.f,  0.f,  0.f}),normalise({1.0f,  1.0f}));
		cubeVertices.at(21) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f},normalise({1.f,  0.f,  0.f}),normalise({1.0f,  0.5f}));
		cubeVertices.at(22) = TPVertexNormalised({ 0.5f,  0.5f, 0.5f },normalise({1.f,  0.f,  0.f}),normalise({0.66f, 0.5f}));
		cubeVertices.at(23) = TPVertexNormalised({ 0.5f, -0.5f, 0.5f },normalise({1.f,  0.f,  0.f}),normalise({0.66f, 1.0f}));


		std::vector<TPVertex> pyramidVertices(16);

		pyramidVertices.at(0) = TPVertex({ -0.5f, -0.5f, -0.5f }, { 0.f, -1.f,  0.f }, {0.f, 0.5f});//  square Magneta
		pyramidVertices.at(1) = TPVertex({ 0.5f, -0.5f, -0.5f }, { 0.f, -1.f , 0.f }, { 0.f, 0.5f });
		pyramidVertices.at(2) = TPVertex({ 0.5f, -0.5f,  0.5f }, { 0.f, -1.f, 0.f }, { 0.33f, 0.5f });
		pyramidVertices.at(3) = TPVertex({ -0.5f, -0.5f,  0.5f }, { 0.f, -1.f, 0.f }, { 0.33f, 0.5f });
		pyramidVertices.at(4) = TPVertex({ -0.5f, -0.5f, -0.5f }, { -0.8944f, 0.4472f, 0.f }, { 0.33f, 0.5f });  //triangle Green
		pyramidVertices.at(5) = TPVertex({ -0.5f, -0.5f,  0.5f }, { -0.8944f, 0.4472f, 0.f }, { 0.66f, 0.25f });
		pyramidVertices.at(6) = TPVertex({ 0.0f,  0.5f,  0.0f }, { -0.8944f, 0.4472f, 0.f }, { 0.33f, 0.f });
		pyramidVertices.at(7) = TPVertex({ -0.5f, -0.5f,  0.5f }, { 0.f, 0.4472f, 0.8944f }, { 0.f, 0.f, }); //triangle Red
		pyramidVertices.at(8) = TPVertex({ 0.5f, -0.5f,  0.5f }, { 0.f, 0.4472f, 0.8944f }, { 0.f, 0.f });
		pyramidVertices.at(9) = TPVertex({ 0.0f,  0.5f,  0.0f }, { 0.f, 0.4472f, 0.8944f }, { 0.f, 0.f });
		pyramidVertices.at(10) = TPVertex({ 0.5f, -0.5f, 0.5f },{  0.8944f, 0.4472f, 0.f}, {0.f, 0.f}); //  triangle Yellow
		pyramidVertices.at(11) = TPVertex({ 0.5f, -0.5f, -0.5f},{  0.8944f, 0.4472f, 0.f}, {0.f, 0.f});
		pyramidVertices.at(12) = TPVertex({ 0.0f,  0.5f,  0.0f},{  0.8944f, 0.4472f, 0.f}, {0.f, 0.f});
		pyramidVertices.at(13) = TPVertex({ 0.5f, -0.5f, -0.5f},{  0.f, 0.4472f, -0.8944f},{ 0.f, 0.f});//  triangle Blue
		pyramidVertices.at(14) = TPVertex({-0.5f, -0.5f, -0.5f},{  0.f, 0.4472f, -0.8944f},{ 0.f, 0.f});
		pyramidVertices.at(15) = TPVertex({ 0.0f,  0.5f,  0.0f},{  0.f, 0.4472f, -0.8944f},{ 0.f, 0.f});
			//	 <------ Pos ------>  <--- normal --->  <--- UV ---> 
					
		

		uint32_t pyramidIndices[3 * 6] =
		{
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12,
			13, 14, 15
		};

		uint32_t cubeIndices[3 * 12] = {
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			12, 13, 14,
			14, 15, 12,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			22, 23, 20
		};
#pragma endregion

#pragma region GL_BUFFERS
		std::shared_ptr<VertexArray> cubeVAO;
		std::shared_ptr<VertexBuffer> cubeVBO;
		std::shared_ptr<IndexBuffer> cubeIBO;


		cubeVAO.reset(VertexArray::create());

		VertexBufferLayout cubeBl = { ShaderDataType::Float3, ShaderDataType::Float3, ShaderDataType::Float2 };
		cubeVBO.reset(VertexBuffer::create(cubeVertices.data(), sizeof(TPVertexNormalised) * cubeVertices.size(), TPVertexNormalised::GetLayout()));

		cubeIBO.reset(IndexBuffer::create(cubeIndices, 36));
		cubeVAO->addVertexBuffer(cubeVBO);
		cubeVAO->setIndexBuffer(cubeIBO);


		std::shared_ptr<VertexArray> pyramidVAO;
		std::shared_ptr<VertexBuffer> pyramidVBO;
		std::shared_ptr<IndexBuffer> pyramidIBO;

		pyramidVAO.reset(VertexArray::create());
		pyramidVBO.reset(VertexBuffer::create(pyramidVertices.data(), sizeof(TPVertex)*pyramidVertices.size(), cubeBl));
		pyramidIBO.reset(IndexBuffer::create(pyramidIndices, 18));
		pyramidVAO->addVertexBuffer(pyramidVBO);
		pyramidVAO->setIndexBuffer(pyramidIBO);

#pragma endregion


#pragma region SHADERS

		std::shared_ptr<Shader> tpShader;
		tpShader.reset(Shader::create("./assets/shaders/texturedPhong.glsl"));

#pragma endregion 

#pragma region TEXTURES
		std::shared_ptr<Texture> letterTexture;
		std::shared_ptr<Texture> numberTexture;
		std::shared_ptr<Texture> cameraTexture;
		std::shared_ptr<Texture> woodTexture;

		letterTexture.reset(Texture::create("assets/textures/letterCube.png"));
		numberTexture.reset(Texture::create("assets/textures/numberCube.png"));
		cameraTexture.reset(Texture::create("assets/textures/video-camera.png"));
		woodTexture.reset(Texture::create("assets/textures/wooden-box.png"));

		unsigned char whitePX[4] = { 255,255,255,255 };
		std::shared_ptr<Texture> plainWhiteTexture;
		plainWhiteTexture.reset(Texture::create(1, 1, 4, whitePX));
		SubTexture letterCube(letterTexture, { 0.f,0.f }, { 1.f,0.5f });
		SubTexture number(letterTexture, { 0.f,0.5f }, { 1.f,1.f });
		SubTexture camera(cameraTexture, { 0.f,0.f }, { 1.f,1.f });
		
#pragma endregion

#pragma region MATERIALS

		std::shared_ptr<Material> pyramidMat;
		std::shared_ptr<Material> letterCubeMat;
		std::shared_ptr<Material> numberCubeMat;
		std::shared_ptr<Material> woodMat;

		pyramidMat.reset(new Material(tpShader, { 0.3f, 0.9f, 4.f, 1.f }));
		letterCubeMat.reset(new Material(tpShader, letterTexture));
		numberCubeMat.reset(new Material(tpShader, numberTexture));
		woodMat.reset(new Material(tpShader, woodTexture));

#pragma endregion

		glm::mat4 anotherCube = glm::translate(glm::mat4(1.0f), glm::vec3(4.f, 0.f, -6.f));

		m_mainCamera.reset(new Camera(m_window->getWidth(), m_window->getHeight(), {0,0,0}));
		m_mainCameraActive = true;
		m_followCamera.reset(new FollowCamera(m_window->getWidth(), m_window->getHeight(), &anotherCube));

		//Camera UBO
		uint32_t blockNumber = 0;

		UniformBufferLayout camLayout = {{"u_view",ShaderDataType::Mat4},{"u_projection",ShaderDataType::Mat4} };
		std::shared_ptr<UniformBuffer> cameraUBO;
		cameraUBO.reset(UniformBuffer::create (camLayout));

		cameraUBO->attachShaderBlock(tpShader, "b_camera");
;
		glm::mat4 models[3];
		models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(-2.f, 0.f, -6.f));
		models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -6.f));
		models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(2.f, 0.f, -6.f));


		glm::mat4 view2D = glm::mat4(1.f);
		glm::mat4 projection2D = glm::ortho(0.f, static_cast<float>(m_window->getWidth()), static_cast<float>(m_window->getHeight()),0.f);

		SceneWideUniforms swu3D;

		glm::vec3 lightData[3] = { {1.f, 1.f, 1.f}, {-2.f, 4.f, 6.f}, {0.f, 0.f, 0.f} };

		swu3D["u_viewPos"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(m_mainCamera->position)));
		swu3D["u_lightColour"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(lightData[0])));
		swu3D["u_lightPos"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(lightData[1])));
		

		SceneWideUniforms swu2D;
		swu2D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));
		swu2D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection2D)));

		float timestep = 0.f;

		{
			std::shared_ptr<RenderCommand> setClearCommand;
			setClearCommand.reset(RenderCommandFactory::createCommand(RenderCommand::Commands::setClearColour,1.f,0.f,1.f,1.f));
			RendererCommon::actionCommand(setClearCommand);
		}

		Renderer3D::init();
		Renderer2D::init();

		std::shared_ptr<RenderCommand> clearCommand;
		clearCommand.reset(RenderCommandFactory::createCommand(RenderCommand::Commands::clearColurAndDepthBuffer));

		std::shared_ptr<RenderCommand> depthTestCommand;
		depthTestCommand.reset(RenderCommandFactory::createCommand(RenderCommand::Commands::enableDepthTest));

		float advance;

		Quad quads[2] =
		{
			Quad::createCentreHalfExtents({ 0, 0 }, { 1024, 100.f }),
			Quad::createCentreHalfExtents({ 950, 50 }, { 50, 50.f }),
		};

		while (m_running)
		{
			timestep = m_timer->getElapsedTime();
			m_timer->reset();

			glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO->GetRenderID());
			cameraUBO->attachShaderBlock(tpShader, "b_camera");

			if (m_mainCameraActive)
			{
				m_mainCamera->Input(timestep);
				cameraUBO->uploadData("u_projection", glm::value_ptr(m_mainCamera->projection));
				cameraUBO->uploadData("u_view", glm::value_ptr(m_mainCamera->view));
				swu3D["u_viewPos"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(m_mainCamera->position)));
			}
			else
			{
				m_followCamera->Input(timestep);
				cameraUBO->uploadData("u_projection", glm::value_ptr(m_followCamera->projection));
				cameraUBO->uploadData("u_view", glm::value_ptr(m_followCamera->view));
				swu3D["u_viewPos"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(m_followCamera->position)));
			}

			//Log::trace("FPS {0}", 1.0f / timestep);

			GLuint uniformLocation;
			for (auto& model : models) { model = glm::rotate(model, timestep, glm::vec3(0.f, 1.0, 0.f)); }

			RendererCommon::actionCommand(clearCommand);
			RendererCommon::actionCommand(depthTestCommand);
			Renderer3D::begin(swu3D);
			Renderer3D::submit(pyramidVAO, pyramidMat, models[0]);
			Renderer3D::submit(cubeVAO, letterCubeMat, models[1]);
			Renderer3D::submit(cubeVAO, numberCubeMat, models[2]);
			Renderer3D::submit(cubeVAO, woodMat, anotherCube);

			Renderer3D::end();

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Renderer2D::begin(swu2D);
			Renderer2D::submit(quads[0], {0.22f,0.22f,0.22f,0.4f});
			Renderer2D::submit(quads[1], {1.f,1.f,1.f,1.f}, camera);
			uint32_t x = 10;

			if (m_mainCameraActive)
			{
				Renderer2D::submit('F', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('r', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('e', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('e', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit(' ', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('L', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('o', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('o', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('k', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit(' ', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('C', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('a', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('m', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
			}
			else
			{
				Renderer2D::submit('F', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('o', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('l', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('l', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('o', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('w', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit(' ', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('C', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('a', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
				Renderer2D::submit('m', { x,80.f }, advance, { 1.f,1.f,1.f,1.f }); x += advance;
			}
			Renderer2D::end();

			glDisable(GL_BLEND);

			m_window->onUpdate(timestep);
		}

	}


	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e)->bool {return this->onWindowResize(e); });
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e)->bool {return this->onWindowClose(e); });
		dispatcher.dispatch<WindowFocusEvent>([this](WindowFocusEvent& e)->bool {return this->onWindowFocus(e); });
		dispatcher.dispatch<WindowMovedEvent>([this](WindowMovedEvent& e)->bool {return this->onWindowMoved(e); });

		dispatcher.dispatch<WindowLostFocusEvent>([this](WindowLostFocusEvent& e)->bool {return this->onWindowLostFocus(e); });
		dispatcher.dispatch<KeyPressedEvent>([this](KeyPressedEvent& e)->bool {return this->onKeyPressed(e); });
		dispatcher.dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& e)->bool {return this->onKeyReleased(e); });
		dispatcher.dispatch<KeyTypedEvent>([this](KeyTypedEvent& e)->bool {return this->onKeyTyped(e); });
		dispatcher.dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e)->bool {return this->onMouseButtonPressed(e); });
		dispatcher.dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e)->bool {return this->onMouseButtonReleased(e); });
		dispatcher.dispatch<MouseMovedEvent>([this](MouseMovedEvent& e)->bool {return this->onMouseMoved(e); });
		dispatcher.dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e)->bool {return this->onMouseScrolled(e); });
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		//Log::info("Window resize event. Width {0}. Height {1}", e.getWidth(), e.getHeight());
		return true;
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		//Log::info("Window closed");
		return true;
	}

	bool Application::onWindowFocus(WindowFocusEvent& e)
	{
		//Log::info("Window Focused");
		return true;
	}

	bool Application::onWindowLostFocus(WindowLostFocusEvent& e)
	{
		//Log::info("Window Lost focus");
		return true;
	}

	bool Application::onWindowMoved(WindowMovedEvent& e)
	{
		//Log::info("Window moved to . Width {0}. Height {1}", e.getxPos(), e.getyPos());
		return true;
	}

	bool Application::onKeyPressed(KeyPressedEvent& e)
	{
		//Log::info("Key  {0} pressed", e.getKeyCode());
		return true;
	}

	bool Application::onKeyReleased(KeyReleasedEvent& e)
	{
		//Log::info("Key  {0} released", e.getKeyCode());
		return true;
	}

	bool Application::onKeyTyped(KeyTypedEvent& e)
	{
		if (e.getKeyCode() == NG_KEY_TAB)
		{
			if (m_mainCameraActive)
				m_mainCameraActive = false;
			else
				m_mainCameraActive = true;
		}

		
		//Log::info("Key  {0} typed", e.getKeyCode());
		return true;
	}

	bool Application::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		//Log::info("Mouse button  {0} pressed", e.getButton());
		return true;
	}

	bool Application::onMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		//Log::info("Mouse button  {0} released", e.getButton());
		return true;
	}

	bool Application::onMouseMoved(MouseMovedEvent& e)
	{
		//Log::info("Mouse  moved by : Width {0}. Height {1}", e.getXoffset(), e.getYoffset());
		return true;
	}

	bool Application::onMouseScrolled(MouseScrolledEvent& e)
	{
		//Log::info("Mouse  scrolled by : Width {0}. Height {1}", e.getXoffset(), e.getYoffset());
		return true;
	}

}
