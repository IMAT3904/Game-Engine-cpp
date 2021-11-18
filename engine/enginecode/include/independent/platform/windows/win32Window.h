/* \file win32Window.h*/
#pragma once

#include "core/window.h"


// Targeting Windows 10 only

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <windows.h>
#include <tchar.h>

namespace Engine
{
	class Win32Window : public Window
	{
	public:
		Win32Window(const WindowProperties& properties) { init(properties); } //!<Constructor
		virtual void init(const WindowProperties& properties) override; //!< Initialise the window
		virtual void close() override; //!< Close the window
		//virtual ~Window() {};
		virtual void onUpdate(float timestep) override;
		virtual void onResize(unsigned int width, unsigned int height) override;
		virtual void setVsync(bool vSync) override;
		virtual void setEventCallback(const std::function<void(Event&)>& callback) override;
		virtual inline unsigned int getWidth() const override { return m_props.height; };
		virtual inline unsigned int getHeight() const override { return m_props.width; };
		virtual void* getNativeWindow() const  override { return m_native; };
		virtual inline bool isFullScreenMode() const  override { return m_props.isFullScreen; };
		virtual inline bool isVSync() const override { return false; };
		LRESULT CALLBACK onWin32Msg(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		HWND m_native; //!< native win32 window
		WindowProperties m_props; //!< window properites
		float m_aspectRatio; //!< Aspect ratio
		std::shared_ptr<GraphicsContext> m_graphicsContext;
		std::function<void(Event&)> m_callback; //General OnEvent call back - bout to application::OnEvent(e)
	};
}
