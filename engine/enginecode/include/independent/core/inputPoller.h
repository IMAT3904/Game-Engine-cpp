/* \file inputPoller.h */
#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	/* \class InputPoller
	* API diagnostic input poller for getting current keyobard/mouse state
	*/
	class InputPoller
	{
	public:
		static bool isKeyPressed(int32_t keyCode); //!< is the key pressed?
		static bool isMouseButtonPressed(int32_t mouseButton); //!< is mouse button pressed?
		static glm::vec2 getMousePosition(); //!< current mouse position
		inline static float getMouseX() { return getMousePosition().x; };
		inline static float getMouseY() { return getMousePosition().y; };
		static void setNativeWindow(void* nativeWin);

	};
}