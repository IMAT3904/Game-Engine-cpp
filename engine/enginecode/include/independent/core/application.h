/** \file application.h
*/
#pragma once

#include "systems/log.h"
#include "systems/timer.h"
#include "systems/event.h"
#include "systems/eventdispatcher.h"
#include "core/window.h"
#include "core/inputPoller.h"
#include "rendering/camera.h"
#include "rendering/followCamera.h"
namespace Engine {

	/**
	\class Application
	\brief Fundemental class of the engine. A singleton which runs the game loop infinitely.
	*/

	class Application
	{
	protected:
		Application(); //!< Constructor
		std::shared_ptr<Log> m_logSystem; //!<Logger instance. Need to create it at start first and shut down as last.
		std::shared_ptr<Timer> m_timer; //!<Timer. Useful for measuring how long an operation will take or creating framerate independent operations.
		std::shared_ptr<System> m_windowsSystem; //!< Windows system.
		std::shared_ptr<Window> m_window; //!< Window.
		void onEvent(Event& e); //!< Called when an event happens.
		std::shared_ptr<Camera> m_mainCamera;//!< Main camera
		std::shared_ptr<FollowCamera> m_followCamera;//!< Follow camera
		bool onWindowResize(WindowResizeEvent& e);//!<Callback that fires when window is being resized.
		bool onWindowClose(WindowCloseEvent& e); //!<Callback that fires when window closes.
		bool onWindowFocus(WindowFocusEvent& e);//!< Callback that fires when findow is clicked (focused).
		bool onWindowLostFocus(WindowLostFocusEvent& e);//!< Callback that fires when another window is clicked (lost focus)>
		bool onWindowMoved(WindowMovedEvent& e);//!< Callback that fires when window is being moved arround.
		bool onKeyPressed(KeyPressedEvent& e);//!< Callback that fires when a key on keyboard is being pressed.
		bool onKeyReleased(KeyReleasedEvent& e);//!< Callback that fires when a key is released.
		bool onKeyTyped(KeyTypedEvent& e); //!< Callback that fires when a key is typed.
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);//!< Callback that fires when mouse button is pressed.
		bool onMouseButtonReleased(MouseButtonReleasedEvent& e);//!< Callback that fires when mouse button is released.
		bool onMouseMoved(MouseMovedEvent& e);//!< Callback that happens when mouse (cursor) is being moved.
		bool onMouseScrolled(MouseScrolledEvent& e);//!< Callback that happens when user uses mouse scroll.
	private:
		static Application* s_instance; //!< Singleton instance of the application
		bool m_running = true; //!< Is the application running?
		bool m_mainCameraActive; //!< Is the main camera activated?
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		inline std::shared_ptr<Window>& getWindow() { return m_window; };//!< Returns active window
		void run(); //!< Main loop
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook
}