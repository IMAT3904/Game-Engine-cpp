/** \file application.cpp
*/

#include "engine_pch.h"
#include "core/application.h"


namespace Engine {
	// Set static vars
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}
		m_logSystem.reset(new Log);
		m_logSystem->start();

		m_timer.reset(new Timer);
		m_timer->start();

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
