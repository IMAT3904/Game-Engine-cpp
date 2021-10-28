/** \file application.cpp
*/

#include "engine_pch.h"
#include <glad/glad.h>
#include "core/application.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWSystem.h"
#endif


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
		float timestep = 0.f;
		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		while (m_running)
		{
			timestep = m_timer->getElapsedTime();
			m_timer->reset();

			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

			if (InputPoller::isKeyPressed(NG_KEY_W))
				Log::info("W pressed");



			m_window->onUpdate(timestep);
		};
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
