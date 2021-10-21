/** \file application.cpp
*/

#include "engine_pch.h"
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
		float accumulate = 0;
		float lastFrameTime = 0;
		while (m_running)
		{
			m_timer->reset();
			if (accumulate > 10)
			{
				WindowCloseEvent c;
				onEvent(c);
				m_running = false;
				WindowResizeEvent e(1280, 720);
				onEvent(e);
				Log::info("Exiting the application");
			}
			
			Log::info("last frame {0} accumulate {1}",lastFrameTime, accumulate);
			lastFrameTime = m_timer->getElapsedTime();
			accumulate += lastFrameTime;
		};
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e)->bool {return this->onWindowResize(e); });
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e)->bool {return this->onWindowClose(e); });
		
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		Log::info("Window resize event. Width {0}. Height {1}", e.getWidth(), e.getHeight());
		return true;
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		Log::info("Window closed");
		return true;
	}

}
