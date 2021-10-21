#pragma once
/* \file GLFWinputPoller.h */
#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
namespace Engine
{
	/* \class InputPoller
	* Input poller for getting current keyobard/mouse state to the GLFW system
	*/
	class GLFWInputPoller
	{
	public:
		static bool isKeyPressed(int32_t keyCode); //!< is the key pressed?
		static bool isMouseButtonPressed(int32_t mouseButton); //!< is mouse button pressed?
		static glm::vec2 getMousePosition(); //!< current mouse position
		inline static float getMouseX() { return getMousePosition().x; };
		inline static float getMouseY() { return getMousePosition().y; };
		static void setCurrentWindow(GLFWwindow* newWin) { s_window = newWin; };
	private:
		static GLFWwindow* s_window; //!< Current GLFW Window
	};
}