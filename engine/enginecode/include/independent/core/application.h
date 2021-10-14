/** \file application.h
*/
#pragma once

#include "systems/log.h"
#include "systems/timer.h"
#include "systems/event.h"
#include "systems/eventdispatcher.h"

namespace Engine {

	/**
	\class Application
	Fundemental class of the engine. A singleton which runs the game loop infinitely.
	Provides ...
	*/

	class Application
	{
	protected:
		Application(); //!< Constructor

		std::shared_ptr<Log> m_logSystem;
		std::shared_ptr<Timer> m_timer;
	private:
		static Application* s_instance; //!< Singleton instance of the application
		bool m_running = true; //!< Is the application running?
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		void run(); //!< Main loop
		void onEvent(Event& e); //!< Called when an event happens
		bool onWindowResize(WindowResizeEvent& e);
		bool onWindowClose(WindowCloseEvent& e);
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook
}