#pragma once
/** \file event.h
*/


namespace Engine
{

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryWindow = 1 << 0,       //00000001
		EventCategoryInput = 1 << 1,        //00000010
		EventCategoryKeyboard = 1 << 2,     //00000100
		EventCategoryMouse = 1 << 3,        //00001000
		EventCategoryMouseButton = 1 << 4,  //00010000
	};

	/**
	\class Event 
	\brief Event base class.
	*/

	class Event
	{
	private:
		bool m_handled = false;
	public:
		virtual EventType getEventType() const = 0; //!< Get the event type
		virtual int getCategoryFlags() const = 0; //!< Get the event category
		inline bool isHandled() const { return m_handled; } //!< Has event been handled
		inline void handle(bool isHandled) { m_handled = isHandled; } //!< Handle the event
		inline bool isInCategory(EventCategory category) { return getCategoryFlags() & category; } //!< Is this event in category?
	};

	/**
	\class WindowCloseEvent
	*/

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		static EventType getStaticType() { return EventType::WindowClose; }
		EventType getEventType() const override { return EventType::WindowClose; }
		int getCategoryFlags() const override { return EventCategoryWindow; }
	};

	/**
	\class WindowResizeEvent
	*/

	class WindowResizeEvent : public Event
	{
	private:
		int m_width;
		int m_height;
	public:
		WindowResizeEvent(int width, int height) : m_width(width), m_height(height) {}
		static EventType getStaticType() { return EventType::WindowResize; }
		virtual EventType getEventType() const override { return EventType::WindowResize; }
		virtual int getCategoryFlags() const override { return EventCategoryWindow; }

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
	};

	/**
	\class WindowFocusEvent
	*/

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}
		static EventType getStaticType() { return EventType::WindowFocus; }
		EventType getEventType() const override { return EventType::WindowFocus; }
		int getCategoryFlags() const override { return EventCategoryWindow; }
	};

	/**
	\class WindowLostFocusEvent
	*/

	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {}
		static EventType getStaticType() { return EventType::WindowLostFocus; }
		EventType getEventType() const override { return EventType::WindowLostFocus; }
		int getCategoryFlags() const override { return EventCategoryWindow; }
	};

	/**
	\class WindowMovedEvent
	*/

	class WindowMovedEvent : public Event
	{
	private:
		float m_xPos;
		float m_yPos;
	public:
		WindowMovedEvent(float xPos, float yPos) : m_xPos(xPos), m_yPos(yPos) {}
		static EventType getStaticType() { return EventType::WindowMoved; }
		virtual EventType getEventType() const override { return EventType::WindowMoved; }
		virtual int getCategoryFlags() const override { return EventCategoryWindow; }

		inline int getxPos() const { return m_xPos; }
		inline int getyPos() const { return m_yPos; }
	};

	/**
	\class KeyPressedEvent
	*/

	class KeyPressedEvent : public Event
	{
	private:
		int m_keyCode;
		int m_repeatCount;
	public:
		KeyPressedEvent(int keycode, int repeatCount) : m_keyCode(keycode), m_repeatCount(repeatCount) {}
		static EventType getStaticType() { return EventType::KeyPressed; }
		virtual EventType getEventType() const override { return EventType::KeyPressed; }
		virtual int getCategoryFlags() const override { return EventCategoryKeyboard; }

		inline int getKeyCode() const { return m_keyCode; }
		inline int getRepeatCount() const { return m_repeatCount; }
	};

	/**
	\class KeyReleasedEvent
	*/

	class KeyReleasedEvent : public Event
	{
	private:
		int m_keyCode;
	public:
		KeyReleasedEvent(int keycode) : m_keyCode(keycode) {}
		static EventType getStaticType() { return EventType::KeyReleased; }
		virtual EventType getEventType() const override { return EventType::KeyReleased; }
		virtual int getCategoryFlags() const override { return EventCategoryKeyboard; }

		inline int getKeyCode() const { return m_keyCode; }
	};

	/*
	\class KeyTypedEvent
	*/

	class KeyTypedEvent : public Event
	{
	private:
		int m_keyCode;
	public:
		KeyTypedEvent(int keycode) : m_keyCode(keycode) {}
		static EventType getStaticType() { return EventType::KeyReleased; }
		virtual EventType getEventType() const override { return EventType::KeyTyped; }
		virtual int getCategoryFlags() const override { return EventCategoryKeyboard; }

		inline int getKeyCode() const { return m_keyCode; }
	};

	/*
	\class MouseButtonPressedEvent
	*/

	class MouseButtonPressedEvent : public Event
	{
	private:
		int m_button;
	public:
		MouseButtonPressedEvent(int button) : m_button(button) {}
		static EventType getStaticType() { return EventType::MouseButtonPressed; }
		virtual EventType getEventType() const override { return EventType::MouseButtonPressed; }
		virtual int getCategoryFlags() const override { return EventCategoryMouse; }

		inline int getButton() const { return m_button; }
	};

	/*
	\class MouseButtonReleasedEvent
	*/

	class MouseButtonReleasedEvent : public Event
	{
	private:
		int m_button;
	public:
		MouseButtonReleasedEvent(int button) : m_button(button) {}
		static EventType getStaticType() { return EventType::MouseButtonReleased; }
		virtual EventType getEventType() const override { return EventType::MouseButtonReleased; }
		virtual int getCategoryFlags() const override { return EventCategoryMouse; }

		inline int getButton() const { return m_button; }
	};

	/*
	\class MouseMovedEvent
	*/

	class MouseMovedEvent : public Event
	{
	private:
		float m_xOffset;
		float m_yOffset;
	public:
		MouseMovedEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}
		static EventType getStaticType() { return EventType::MouseMoved; }
		virtual EventType getEventType() const override { return EventType::MouseMoved; }
		virtual int getCategoryFlags() const override { return EventCategoryMouse; }

		inline int getXoffset() const { return m_xOffset; }
		inline int getYoffset() const { return m_yOffset; }
	};

	/*
	\class MouseScrolledEvent
	*/

	class MouseScrolledEvent : public Event
	{
	private:
		float m_xOffset;
		float m_yOffset;
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}
		static EventType getStaticType() { return EventType::MouseScrolled; }
		virtual EventType getEventType() const override { return EventType::MouseScrolled; }
		virtual int getCategoryFlags() const override { return EventCategoryMouse; }

		inline int getXoffset() const { return m_xOffset; }
		inline int getYoffset() const { return m_yOffset; }
	};

}
