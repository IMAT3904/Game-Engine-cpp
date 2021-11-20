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

		WindowProperties props("My game window", 800, 600, 0, 0);
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


	void Application::run()
	{
#pragma region RAW_DATA

		float cubeVertices[8 * 24] = {
			//	 <------ Pos ------>  <--- normal --->  <-- UV -->
				 0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,   0.f,
				 0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,   0.5f,
				-0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.33f, 0.5f,
				-0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.33f, 0.f,

				-0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  0.33f, 0.5f,
				 0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  0.66f, 0.5f,
				 0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  0.66f, 0.f,
				-0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  0.33,  0.f,

				-0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  1.f,   0.f,
				 0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  0.66f, 0.f,
				 0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  0.66f, 0.5f,
				-0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  1.0f,  0.5f,

				 0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,  0.f,   0.5f,
				 0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.f,   1.0f,
				-0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.33f, 1.0f,
				-0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,  0.3f,  0.5f,

				-0.5f,  0.5f, 0.5f,  -1.f,  0.f,  0.f,  0.66f, 0.5f,
				-0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,  0.33f, 0.5f,
				-0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  0.33f, 1.0f,
				-0.5f, -0.5f, 0.5f,  -1.f,  0.f,  0.f,  0.66f, 1.0f,

				 0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  0.5f,
				 0.5f,  0.5f, 0.5f,   1.f,  0.f,  0.f,  0.66f, 0.5f,
				 0.5f, -0.5f, 0.5f,   1.f,  0.f,  0.f,  0.66f, 1.0f
		};

		float pyramidVertices[8 * 16] = {
			//	 <------ Pos ------>  <--- normal --->  <--- UV ---> 
				-0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f,  0.f,0.5f,//  square Magneta
				 0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f,  0.f, 0.5f,
				 0.5f, -0.5f,  0.5f, 0.f, -1.f, 0.f,  0.33f, 0.5f,
				-0.5f, -0.5f,  0.5f, 0.f, -1.f, 0.f,  0.33f, 0.5f,

				-0.5f, -0.5f, -0.5f,  -0.8944f, 0.4472f, 0.f, 0.33f, 0.5f,  //triangle Green
				-0.5f, -0.5f,  0.5f,  -0.8944f, 0.4472f, 0.f, 0.66f, 0.25f,
				 0.0f,  0.5f,  0.0f,  -0.8944f, 0.4472f, 0.f, 0.33f, 0.f,

				-0.5f, -0.5f,  0.5f,  0.f, 0.4472f, 0.8944f, 0.f, 0.f, //triangle Red
				 0.5f, -0.5f,  0.5f,  0.f, 0.4472f, 0.8944f, 0.f, 0.f,
				 0.0f,  0.5f,  0.0f,  0.f, 0.4472f, 0.8944f, 0.f, 0.f,

				 0.5f, -0.5f,  0.5f,  0.8944f, 0.4472f, 0.f, 0.f, 0.f, //  triangle Yellow
				 0.5f, -0.5f, -0.5f,  0.8944f, 0.4472f, 0.f, 0.f, 0.f,
				 0.0f,  0.5f,  0.0f,  0.8944f, 0.4472f, 0.f, 0.f, 0.f,

				 0.5f, -0.5f, -0.5f,  0.f, 0.4472f, -0.8944f, 0.f, 0.f,//  triangle Blue
				-0.5f, -0.5f, -0.5f,  0.f, 0.4472f, -0.8944f, 0.f, 0.f,
				 0.0f,  0.5f,  0.0f,  0.f, 0.4472f, -0.8944f, 0.f, 0.f
		};

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

		BufferLayout cubeBl = { ShaderDataType::Float3, ShaderDataType::Float3, ShaderDataType::Float2 };
		cubeVBO.reset(VertexBuffer::create(cubeVertices, sizeof(cubeVertices), cubeBl));

		cubeIBO.reset(IndexBuffer::create(cubeIndices, 36));
		cubeVAO->addVertexBuffer(cubeVBO);
		cubeVAO->setIndexBuffer(cubeIBO);


		std::shared_ptr<VertexArray> pyramidVAO;
		std::shared_ptr<VertexBuffer> pyramidVBO;
		std::shared_ptr<IndexBuffer> pyramidIBO;

		pyramidVAO.reset(VertexArray::create());
		pyramidVBO.reset(VertexBuffer::create(pyramidVertices, sizeof(pyramidVertices), cubeBl));
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

		letterTexture.reset(Texture::create("assets/textures/letterCube.png"));
		numberTexture.reset(Texture::create("assets/textures/numberCube.png"));

		unsigned char whitePX[4] = { 255,255,255,255 };
		std::shared_ptr<Texture> plainWhiteTexture;
		plainWhiteTexture.reset(Texture::create(1, 1, 4, whitePX));
		SubTexture letterCube(letterTexture, { 0.f,0.f }, { 1.f,0.5f });
		SubTexture number(letterTexture, { 0.f,0.5f }, { 1.f,1.f });
		
#pragma endregion

#pragma region MATERIALS

		std::shared_ptr<Material> pyramidMat;
		std::shared_ptr<Material> letterCubeMat;
		std::shared_ptr<Material> numberCubeMat;

		pyramidMat.reset(new Material(tpShader, { 0.3f, 0.9f, 4.f, 1.f }));
		letterCubeMat.reset(new Material(tpShader, letterTexture));
		numberCubeMat.reset(new Material(tpShader, numberTexture));
		


#pragma endregion

		glm::mat4 view = glm::lookAt(
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.f, 1.f, 0.f)
		);
		glm::mat4 projection = glm::perspective(glm::radians(45.f), 1024.f / 800.f, 0.1f, 100.f);

		glm::mat4 models[3];
		models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(-2.f, 0.f, -6.f));
		models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -6.f));
		models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(2.f, 0.f, -6.f));

		SceneWideUniforms swu3D;

		glm::vec3 lightData[3] = { {1.f, 1.f, 1.f}, {-2.f, 4.f, 6.f}, {0.f, 0.f, 0.f} };

		swu3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view)));
		swu3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection)));
		swu3D["u_lightColour"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(lightData[0])));
		swu3D["u_lightPos"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(lightData[1])));
		swu3D["u_viewPos"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(lightData[2])));



		float timestep = 0.f;

		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

		Renderer3D::init();
		while (m_running)
		{
			timestep = m_timer->getElapsedTime();
			m_timer->reset();
			//Log::trace("FPS {0}", 1.0f / timestep);
			//if (InputPoller::isKeyPressed(NG_KEY_W)) Log::error("W Pressed");
			//if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_1)) Log::error("Left Mouse Button Pressed");
			//Log::trace("Current mouse pos: ({0}, {1})", InputPoller::getMouseX(), InputPoller::getMouseY());

			GLuint uniformLocation;
			for (auto& model : models) { model = glm::rotate(model, timestep, glm::vec3(0.f, 1.0, 0.f)); }

			// Do frame stuff
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Renderer3D::begin(swu3D);

			Renderer3D::submit(pyramidVAO, pyramidMat, models[0]);
			Renderer3D::submit(cubeVAO, letterCubeMat, models[1]);
			Renderer3D::submit(cubeVAO, numberCubeMat, models[2]);

			Renderer3D::end();

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
