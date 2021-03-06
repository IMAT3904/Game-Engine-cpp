/** \file inputPoller.cpp*/
#include "engine_pch.h"
#include "core/inputPoller.h"

#ifdef NG_PLATFORM_WINDOWS
//#include "platform/windows/win32InputPoller.h"
//#else
#include "platform//GLFW/GLFWInputPoller.h"
#include <GLFW/glfw3.h>
#endif

namespace Engine 
{
//#ifdef NG_PLATFORM_WINDOWS
//	bool InputPoller::isKeyPressed(int32_t keyCode)
//	{
//		return Win32InputPoller::isKeyPressed(keyCode);
//	}
//	bool InputPoller::isMouseButtonPressed(int32_t mouseButton)
//	{
//		return Win32InputPoller::isMouseButtonPressed(mouseButton);
//	}
//	glm::vec2 InputPoller::getMousePosition()
//	{
//		return Win32InputPoller::getMousePosition();
//	}
//	void InputPoller::setNativeWindow(void* nativeWin)
//	{
//
//	}
//#endif
#ifdef NG_PLATFORM_WINDOWS
	bool InputPoller::isKeyPressed(int32_t keyCode)
	{
		return GLFWInputPoller::isKeyPressed(keyCode);
	}
	bool InputPoller::isMouseButtonPressed(int32_t mouseButton)
	{
		return GLFWInputPoller::isMouseButtonPressed(mouseButton);
	}
	glm::vec2 InputPoller::getMousePosition()
	{
		return GLFWInputPoller::getMousePosition();
	}
	void InputPoller::setNativeWindow(void* nativeWin)
	{
		GLFWInputPoller::setCurrentWindow(reinterpret_cast<GLFWwindow*>(nativeWin));
	}
#endif
}