/* \file win32InputPoller.cpp*/

#include "engine_pch.h"
#include "platform/windows/win32InputPoller.h"
namespace Engine
{
	bool Win32InputPoller::isKeyPressed(int32_t keyCode)
	{
		int32_t result = GetAsyncKeyState(keyCode);
		return result & NG_KEY_PRESS;
	}
	bool Win32InputPoller::isMouseButtonPressed(int32_t mouseButton)
	{
		int32_t result = GetAsyncKeyState(mouseButton);
		return result & NG_KEY_PRESS;
	}
	glm::vec2 Win32InputPoller::getMousePosition()
	{
		LPPOINT point;
		if (GetCursorPos(point))
		{
			return glm::vec2(static_cast<float>(point->x), static_cast<float>(point->y));
		}
		return glm::vec2();
	}
}