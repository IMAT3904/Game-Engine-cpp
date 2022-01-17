/* \file  win32System.h*/
#pragma once

#include "systems/system.h"
#include "core/application.h"

namespace Engine
{
	/**
	\class Win32System
	\brief Win32 implementation of system class.
	*/
	class Win32System : public System
	{
		virtual void start(SystemSignal init = SystemSignal::None, ...) override; //!< Start the system
		virtual void stop(SystemSignal close = SystemSignal::None, ...) override; //!< Stop the system
	};
}
