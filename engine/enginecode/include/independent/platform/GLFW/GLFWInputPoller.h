#pragma once
/* \file GLFWinputPoller.h */

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
namespace Engine
{
	/* \class InputPoller
	* \brief Input poller that getts current keyobard/mouse state from GLFW system.
	*/
	class GLFWInputPoller
	{
	public:
		static bool isKeyPressed(int32_t keyCode); //!< is the key pressed?
		static bool isMouseButtonPressed(int32_t mouseButton); //!< is mouse button pressed?
		static glm::vec2 getMousePosition(); //!< current mouse position
		inline static float getMouseX() { return getMousePosition().x; }; //!< Get current mouse X position.
		inline static float getMouseY() { return getMousePosition().y; }; //!< Get current mouse Y position.
		static void setCurrentWindow(GLFWwindow* newWin) { s_window = newWin; }; //!< Change current window to a new one.
	private:
		static GLFWwindow* s_window; //!< Current GLFW Window
	};
}