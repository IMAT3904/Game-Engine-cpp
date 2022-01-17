/* \file win32Window.h*/
#pragma once
#include "core/window.h"


// Targeting Windows 10 and Windows 11 only

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <windows.h>
#include <tchar.h>

namespace Engine
{
	/**
	\class Win32Window
	\brief Win32 implementation of Window class.
	*/
	class Win32Window : public Window
	{
	public:
		Win32Window(const WindowProperties& properties) { init(properties); } //!<Constructor.
		virtual void init(const WindowProperties& properties) override; //!< Initialise the window.
		virtual void close() override; //!< Close the window.
		//virtual ~Window() {};
		virtual void onUpdate(float timestep) override; //!< Update window logic.
		virtual void onResize(unsigned int width, unsigned int height) override; //!< Callback that fires when window is being resized.
		virtual void setVsync(bool vSync) override; //!< Sets V-Sync.
		virtual void setEventCallback(const std::function<void(Event&)>& callback) override; //!< Set event callback.
		virtual inline unsigned int getWidth() const override { return m_props.height; }; //!< Get window width.
		virtual inline unsigned int getHeight() const override { return m_props.width; }; //!< Get window height.
		virtual void* getNativeWindow() const  override { return m_native; }; //!< Get the native window.
		virtual inline bool isFullScreenMode() const  override { return m_props.isFullScreen; }; //!< Is window in fullscreen mode?
		virtual inline bool isVSync() const override { return false; }; //!< Is V-Sync enabled?
		LRESULT CALLBACK onWin32Msg(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam); //!< Callback required if using win32.
	private:
		HWND m_native; //!< native win32 window
		WindowProperties m_props; //!< window properites
		float m_aspectRatio; //!< Aspect ratio
		std::shared_ptr<GraphicsContext> m_graphicsContext; //!< Grapcs context
		std::function<void(Event&)> m_callback; //!<General OnEvent call back - bout to application::OnEvent(e)
	};
}
