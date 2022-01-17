#pragma once
/** \file GLFWSystem.h  */

#include <GLFW/glfw3.h>
#include "systems/system.h"
#include "systems/log.h"

namespace Engine
{
	/**
	\class GLFWSystem
	\brief GLFW System class that initialises and stops the system.
	*/
	class GLFWSystem : public System
	{
		virtual void start(SystemSignal init = SystemSignal::None, ...) override //!<Start the system
		{
			auto errorCode = glfwInit();
			if (!errorCode)
			{
				Log::info("Cannot init GLFW: {0}", errorCode);
			}
		}
		virtual void stop(SystemSignal close = SystemSignal::None, ...) ///!< Stop the system
		{
			glfwTerminate();
		}
	};
}