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
				m_running = false;
				Log::info("Exiting the application");
			}
			
			Log::info("last frame {0} accumulate {1}",lastFrameTime, accumulate);
			lastFrameTime = m_timer->getElapsedTime();
			accumulate += lastFrameTime;
		};
	}

}
