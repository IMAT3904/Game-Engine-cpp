/** \file timier.h
*/

#include <chrono>

#pragma once
namespace Engine 
{
	/**
	\class Timer
	\brief Basic timer implementation for profiling performance.
	*/
	class Timer
	{
	public:
		void start() //!< Start the timer.
		{
			m_startPoint = std::chrono::high_resolution_clock::now();
		}
		void reset() //!< Reset the timer.
		{
			m_startPoint = std::chrono::high_resolution_clock::now();
		}
		float getElapsedTime() //get elapsed time.
		{
			m_endPoint = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float, std::milli> elapsed = m_endPoint - m_startPoint;
			return elapsed.count() / 1000.f;
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint; //!<Time when starting.
		std::chrono::time_point<std::chrono::high_resolution_clock> m_endPoint; //!<  Time when ending.
	};
}