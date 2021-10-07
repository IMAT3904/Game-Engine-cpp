/** \file log.cpp */
#include "engine_pch.h"
#include "systems/log.h"

namespace Engine 
{
	std::shared_ptr<spdlog::logger> Log::s_consoleLogger = nullptr;

	void Log::start(SystemSignal init, ...)
	{
		//running = true;
		spdlog::set_pattern("%^[%T]: %v%$");
		spdlog::set_level(spdlog::level::trace);

		s_consoleLogger = spdlog::stderr_color_mt("Console");
	}
	void Log::stop(SystemSignal close, ...)
	{
		//running = false;
		s_consoleLogger->info("Stopping console logger");
		s_consoleLogger.reset();
	}
}