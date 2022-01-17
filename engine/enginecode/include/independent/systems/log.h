/** \file log.h
*/
#pragma once

#include "spdlog/spdlog.h"
#include "systems/system.h"
#include <iostream>

namespace Engine
{
	/**
	\class Log
	\brief System logger that writes application messages to console.
	*/
	class Log : public System
	{
	public:
		virtual void start(SystemSignal init = SystemSignal::None, ...) override; //!< Start the logger
		virtual void stop(SystemSignal close = SystemSignal::None, ...) override; //!< Stop the logger

		template<class ...Args>
		static void info(Args&&... args); //!< Display information message.

		template<class ...Args>
		static void debug(Args&... args); //!< Display debug message.

		template<class ...Args>
		static void error(Args&... args); //!< Display error message.

		template<class ...Args>
		static void warn(Args&... args); //!< Display warning message.

		template<class ...Args>
		static void trace(Args&... args); //!< Trace message.

		static bool running; //!< Is the logger running?

	private:
		static std::shared_ptr<spdlog::logger> s_consoleLogger; //!< Console logger instance.
	};

	template<class ...Args>
	static void Log::debug(Args&... args)
	{
		s_consoleLogger->debug(std::forward<Args>(args));
	}

	template<class ...Args>
	inline void Log::error(Args& ...args)
	{
		s_consoleLogger->error(std::forward<Args>(args) ...);
	}

	template<class ...Args>
	inline void Log::warn(Args& ...args)
	{
		s_consoleLogger->warn(std::forward<Args>(args) ...);
	}

	template<class ...Args>
	inline void Log::trace(Args& ...args)
	{
		s_consoleLogger->trace(std::forward<Args>(args) ...);
	}

	template<class ...Args>
	static void Log::info(Args&&... args)
	{
		s_consoleLogger->info(std::forward<Args>(args) ...);
	}

}
