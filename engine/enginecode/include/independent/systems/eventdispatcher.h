#pragma once
#include <functional>
#include "systems/event.h"

#ifdef NG_PLATFORM_WINDOWS
#include "systems/GLFWCodes.h"
//#else
//#include "systems/win32Codes.h"
#endif // NG_PLATFORM_WINDOWS



namespace Engine 
{
	/**
	\class EventDispatcher
	\brief Dispatch all event application.
	*/
	class EventDispatcher
	{
		template<typename T>
		using EventFunc = std::function<bool(T&)>;
	private:
		Event& m_event;
	public:
		EventDispatcher(Event& event) : m_event(event)
		{
		}

		template<typename T>
		bool dispatch(EventFunc<T> func)
		{
			if (m_event.getEventType() == T::getStaticType())
			{
				m_event.handle(func(*((T*)&m_event)));
				return true;
			}
			return false;
		}
	};
}
