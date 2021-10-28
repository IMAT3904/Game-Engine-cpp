/** \file application.cpp
*/

#include "engine_pch.h"
#include <glad/glad.h>
#include "core/application.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWSystem.h"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "platform/OpenGL/OpenGLVertexArray.h"

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

		WindowProperties props("My game window",800,600,0,0);
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
		//Stop logging
		m_logSystem->stop();

		//Stop windows
		m_windowsSystem->stop();
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

		float pyramidVertices[6 * 16] = {
			//	 <------ Pos ------>  <--- colour ---> 
				-0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.8f, //  square Magneta
				 0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.8f,
				 0.5f, -0.5f,  0.5f,  0.8f, 0.2f, 0.8f,
				-0.5f, -0.5f,  0.5f,  0.8f, 0.2f, 0.8f,

				-0.5f, -0.5f, -0.5f,  0.2f, 0.8f, 0.2f,  //triangle Green
				-0.5f, -0.5f,  0.5f,  0.2f, 0.8f, 0.2f,
				 0.0f,  0.5f,  0.0f,  0.2f, 0.8f, 0.2f,

				-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.f, //triangle Red
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.f,
				 0.0f,  0.5f,  0.0f,  1.0f, 0.0f, 0.f,

				 0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.2f, //  triangle Yellow
				 0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.2f,
				 0.0f,  0.5f,  0.0f,  0.8f, 0.8f, 0.2f,

				 0.5f, -0.5f, -0.5f,  0.f, 0.2f, 1.0f,//  triangle Blue
				-0.5f, -0.5f, -0.5f,  0.f, 0.2f, 1.0f,
				 0.0f,  0.5f,  0.0f,  0.f, 0.2f, 1.0f
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
		std::shared_ptr<OpenGLVertexArray> cubeVAO;
		std::shared_ptr<OpenGLVertexBuffer> cubeVBO;
		std::shared_ptr<OpenGLIndexBuffer> cubeIBO;


		cubeVAO.reset(new OpenGLVertexArray);
		
		BufferLayout cubeBl = { ShaderDataType::Float3, ShaderDataType::Float3, ShaderDataType::Float2 };
		cubeVBO.reset(new OpenGLVertexBuffer(cubeVertices,sizeof(cubeVertices), cubeBl));

		cubeIBO.reset(new OpenGLIndexBuffer(cubeIndices, 36));
		cubeVAO->addVertexBuffer(cubeVBO);
		cubeVAO->setIndexBuffer(cubeIBO);


		std::shared_ptr<OpenGLVertexArray> pyramidVAO;
		std::shared_ptr<OpenGLVertexBuffer> pyramidVBO;
		std::shared_ptr<OpenGLIndexBuffer> pyramidIBO;

		pyramidVAO.reset(new OpenGLVertexArray);
		BufferLayout pyramidBL = { ShaderDataType::Float3, ShaderDataType::Float3 };
		pyramidVBO.reset(new OpenGLVertexBuffer(pyramidVertices, sizeof(pyramidVertices), pyramidBL));
		pyramidIBO.reset(new OpenGLIndexBuffer(pyramidIndices, 18));
		pyramidVAO->addVertexBuffer(pyramidVBO);
		pyramidVAO->setIndexBuffer(pyramidIBO);
		
#pragma endregion


#pragma region SHADERS
		std::string FCvertSrc = R"(
				#version 440 core
			
				layout(location = 0) in vec3 a_vertexPosition;
				layout(location = 1) in vec3 a_vertexColour;
				out vec3 fragmentColour;
				uniform mat4 u_model;
				uniform mat4 u_view;
				uniform mat4 u_projection;
				void main()
				{
					fragmentColour = a_vertexColour;
					gl_Position =  u_projection * u_view * u_model * vec4(a_vertexPosition,1);
				}
			)";

		std::string FCFragSrc = R"(
				#version 440 core
			
				layout(location = 0) out vec4 colour;
				in vec3 fragmentColour;
				void main()
				{
					colour = vec4(fragmentColour, 1.0);
				}
		)";

		std::string TPvertSrc = R"(
				#version 440 core
			
				layout(location = 0) in vec3 a_vertexPosition;
				layout(location = 1) in vec3 a_vertexNormal;
				layout(location = 2) in vec2 a_texCoord;
				out vec3 fragmentPos;
				out vec3 normal;
				out vec2 texCoord;
				uniform mat4 u_model;
				uniform mat4 u_view;
				uniform mat4 u_projection;
				void main()
				{
					fragmentPos = vec3(u_model * vec4(a_vertexPosition, 1.0));
					normal = mat3(transpose(inverse(u_model))) * a_vertexNormal;
					texCoord = vec2(a_texCoord.x, a_texCoord.y);
					gl_Position =  u_projection * u_view * u_model * vec4(a_vertexPosition,1.0);
				}
			)";

		std::string TPFragSrc = R"(
				#version 440 core
			
				layout(location = 0) out vec4 colour;
				in vec3 normal;
				in vec3 fragmentPos;
				in vec2 texCoord;
				uniform vec3 u_lightPos; 
				uniform vec3 u_viewPos; 
				uniform vec3 u_lightColour;
				uniform sampler2D u_texData;
				void main()
				{
					float ambientStrength = 0.4;
					vec3 ambient = ambientStrength * u_lightColour;
					vec3 norm = normalize(normal);
					vec3 lightDir = normalize(u_lightPos - fragmentPos);
					float diff = max(dot(norm, lightDir), 0.0);
					vec3 diffuse = diff * u_lightColour;
					float specularStrength = 0.8;
					vec3 viewDir = normalize(u_viewPos - fragmentPos);
					vec3 reflectDir = reflect(-lightDir, norm);  
					float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
					vec3 specular = specularStrength * spec * u_lightColour;  
					
					colour = vec4((ambient + diffuse + specular), 1.0) * texture(u_texData, texCoord);
				}
		)";



		uint32_t FCprogram, TPprogram;

		GLuint FCVertShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = FCvertSrc.c_str();
		glShaderSource(FCVertShader, 1, &source, 0);
		glCompileShader(FCVertShader);

		GLint isCompiled = 0;
		glGetShaderiv(FCVertShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(FCVertShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(FCVertShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(FCVertShader);
			return;
		}

		GLuint FCFragShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = FCFragSrc.c_str();
		glShaderSource(FCFragShader, 1, &source, 0);
		glCompileShader(FCFragShader);

		glGetShaderiv(FCFragShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(FCFragShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(FCFragShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(FCFragShader);
			glDeleteShader(FCVertShader);

			return;
		}

		FCprogram = glCreateProgram();
		glAttachShader(FCprogram, FCVertShader);
		glAttachShader(FCprogram, FCFragShader);
		glLinkProgram(FCprogram);

		GLint isLinked = 0;
		glGetProgramiv(FCprogram, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(FCprogram, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(FCprogram, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(FCprogram);
			glDeleteShader(FCVertShader);
			glDeleteShader(FCFragShader);

			return;
		}

		glDetachShader(FCprogram, FCVertShader);
		glDetachShader(FCprogram, FCFragShader);


		GLuint TPVertShader = glCreateShader(GL_VERTEX_SHADER);

		source = TPvertSrc.c_str();
		glShaderSource(TPVertShader, 1, &source, 0);
		glCompileShader(TPVertShader);

		isCompiled = 0;
		glGetShaderiv(TPVertShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(TPVertShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(TPVertShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(TPVertShader);
			return;
		}

		GLuint TPFragShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = TPFragSrc.c_str();
		glShaderSource(TPFragShader, 1, &source, 0);
		glCompileShader(TPFragShader);

		glGetShaderiv(TPFragShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(TPFragShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(TPFragShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(TPFragShader);
			glDeleteShader(TPVertShader);

			return;
		}

		TPprogram = glCreateProgram();
		glAttachShader(TPprogram, TPVertShader);
		glAttachShader(TPprogram, TPFragShader);
		glLinkProgram(TPprogram);

		isLinked = 0;
		glGetProgramiv(TPprogram, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(TPprogram, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(TPprogram, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(TPprogram);
			glDeleteShader(TPVertShader);
			glDeleteShader(TPFragShader);

			return;
		}

		glDetachShader(TPprogram, TPVertShader);
		glDetachShader(TPprogram, TPFragShader);
#pragma endregion 

#pragma region TEXTURES

		uint32_t letterTexture, numberTexture;

		glGenTextures(1, &letterTexture);
		glBindTexture(GL_TEXTURE_2D, letterTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, channels;

		/* Need to add
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
*/
		unsigned char* data = stbi_load("../assets/textures/letterCube.png", &width, &height, &channels, 0);
		if (data)
		{
			if (channels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (channels == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else return;
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			return;
		}
		stbi_image_free(data);

		glGenTextures(1, &numberTexture);
		glBindTexture(GL_TEXTURE_2D, numberTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		data = stbi_load("..\\assets\\textures\\numberCube.png", &width, &height, &channels, 0);
		if (data)
		{
			if (channels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (channels == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else return;
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			return;
		}
		stbi_image_free(data);
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

		float timestep = 0.f;

		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		while (m_running)
		{
			timestep = m_timer->getElapsedTime();
			m_timer->reset();
			//Log::trace("FPS {0}", 1.0f / timestep);
			//if (InputPoller::isKeyPressed(NG_KEY_W)) Log::error("W Pressed");
			//if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_1)) Log::error("Left Mouse Button Pressed");
			//Log::trace("Current mouse pos: ({0}, {1})", InputPoller::getMouseX(), InputPoller::getMouseY());

			for (auto& model : models) { model = glm::rotate(model, timestep, glm::vec3(0.f, 1.0, 0.f)); }

			// Do frame stuff
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(FCprogram);
			glBindVertexArray(pyramidVAO->getRenderID());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidIBO->getRenderID());

			GLuint uniformLocation;

			uniformLocation = glGetUniformLocation(FCprogram, "u_model");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[0])); // Must include <glm/gtc/type_ptr.hpp>

			uniformLocation = glGetUniformLocation(FCprogram, "u_view");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));

			uniformLocation = glGetUniformLocation(FCprogram, "u_projection");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

			glDrawElements(GL_TRIANGLES, pyramidVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

			glUseProgram(TPprogram);
			glBindVertexArray(cubeVAO->getRenderID());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO->getRenderID());


			uniformLocation = glGetUniformLocation(TPprogram, "u_model");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[1]));

			uniformLocation = glGetUniformLocation(TPprogram, "u_view");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));

			uniformLocation = glGetUniformLocation(TPprogram, "u_projection");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

			uniformLocation = glGetUniformLocation(TPprogram, "u_lightColour");
			glUniform3f(uniformLocation, 1.f, 1.f, 1.f);

			uniformLocation = glGetUniformLocation(TPprogram, "u_lightPos");
			glUniform3f(uniformLocation, 1.f, 4.f, 6.f);

			uniformLocation = glGetUniformLocation(TPprogram, "u_viewPos");
			glUniform3f(uniformLocation, 0.f, 0.f, 0.f);

			glBindTexture(GL_TEXTURE_2D, letterTexture);
			uniformLocation = glGetUniformLocation(TPprogram, "u_texData");
			glUniform1i(uniformLocation, 0);

			glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

			uniformLocation = glGetUniformLocation(TPprogram, "u_model");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[2]));

			glBindTexture(GL_TEXTURE_2D, numberTexture);

			glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

			m_window->onUpdate(timestep);
		}

		glDeleteShader(FCprogram);
		glDeleteShader(TPprogram);

		glDeleteTextures(1, &letterTexture);
		glDeleteTextures(1, &numberTexture);

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
