/** \file application.h
*/
#pragma once

#include "systems/log.h"
#include "systems/timer.h"
#include "systems/event.h"
#include "systems/eventdispatcher.h"
#include "core/window.h"
#include "core/inputPoller.h"

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
		std::shared_ptr<System> m_windowsSystem; //!< Windows system
		std::shared_ptr<Window> m_window; //!< Window
				void onEvent(Event& e); //!< Called when an event happens
		bool onWindowResize(WindowResizeEvent& e);
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowFocus(WindowFocusEvent& e);
		bool onWindowLostFocus(WindowLostFocusEvent& e);
		bool onWindowMoved(WindowMovedEvent& e);
		bool onKeyPressed(KeyPressedEvent& e);
		bool onKeyReleased(KeyReleasedEvent& e);
		bool onKeyTyped(KeyTypedEvent& e);
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool onMouseMoved(MouseMovedEvent& e);
		bool onMouseScrolled(MouseScrolledEvent& e);
	private:
		static Application* s_instance; //!< Singleton instance of the application
		bool m_running = true; //!< Is the application running?
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		inline std::shared_ptr<Window>& getWindow() { return m_window; };
		void run(); //!< Main loop





	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook
}