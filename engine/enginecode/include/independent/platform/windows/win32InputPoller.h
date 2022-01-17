/* \file win32InputPoller.h*/
#pragma once
#include <glm/glm.hpp>
#include "systems/win32Codes.h"
#include <Windows.h>
namespace Engine
{
	/**
	\class Win32InputPoller
	\brief Win32 implementation of Input poller.
	*/
	class Win32InputPoller
	{
	public:
		static bool isKeyPressed(int32_t keyCode); //!< is the key pressed?
		static bool isMouseButtonPressed(int32_t mouseButton); //!< is mouse button pressed?
		static glm::vec2 getMousePosition(); //!< current mouse position
	};
}