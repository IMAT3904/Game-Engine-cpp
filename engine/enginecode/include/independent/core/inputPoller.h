/* \file inputPoller.h */
#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	/* \class InputPoller
	* \brief API diagnostic input poller for getting current keyobard/mouse state.
	*/
	class InputPoller
	{
	public:
		static bool isKeyPressed(int32_t keyCode); //!< Is the key pressed?
		static bool isMouseButtonPressed(int32_t mouseButton); //!< Is mouse button pressed?
		static glm::vec2 getMousePosition(); //!< Returns current mouse position.
		inline static float getMouseX() { return getMousePosition().x; }; //!< Returns mouse X position.
		inline static float getMouseY() { return getMousePosition().y; }; //!< Retunrs mouse Y position.
		static void setNativeWindow(void* nativeWin);//Changes native window. 
	};
}