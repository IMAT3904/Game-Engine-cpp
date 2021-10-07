/** \file log.h
*/
#pragma once

#include "spdlog/spdlog.h"
#include "system.h"
#include <iostream>

namespace Engine
{
	class Log : public System
	{
	public:
		virtual void start(SystemSignal init = SystemSignal::None, ...); //!< Start the logger
		virtual void stop(SystemSignal close = SystemSignal::None, ...); //!< Stop the logger

		template<class ...Args>
		static void info(Args&&... args);

		template<class ...Args>
		static void debug(Args&... args);

		template<class ...Args>
		static void trace(Args&... args);

		static bool running;

	private:
		static std::shared_ptr<spdlog::logger> s_consoleLogger; //!< Console
	};

	template<class ...Args>
	static void Log::debug(Args&... args)
	{
		s_consoleLogger->debug(std::forward<Args>(args));
	}

	template<class ...Args>
	static void Log::info(Args&&... args)
	{
		s_consoleLogger->info(std::forward<Args>(args) ...);
	}
}
