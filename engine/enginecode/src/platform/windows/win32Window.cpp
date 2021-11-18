/* \win32Window.cpp */

#include "engine_pch.h"
#include "platform/windows/win32Window.h"
#include "platform/windows/win32Window.h"
#include "platform/windows/win32_OpenGL_GC.h"
#include "systems/log.h"

namespace Engine
{



	void Win32Window::init(const WindowProperties& properties)
	{
		wchar_t wStr[256];
		mbstowcs(wStr, properties.title, 256);
		

		int32_t x = 0;
		int32_t y = 0;
		
		if (properties.isFullScreen)
		{
			x = 0;
			y = 0;
		}
		else
		{
			x = GetSystemMetrics(SM_CXSCREEN) / 2 - properties.width /2;
			y = GetSystemMetrics(SM_CYSCREEN) / 2 - properties.height /2;
		}
		//Create a window
		
		
		m_native = CreateWindow(
			L"NGWin32Window",
			wStr,
			WS_OVERLAPPEDWINDOW,
			x, y,
			properties.width, properties.height,
			0, 0,
			GetModuleHandle(NULL),
			0
		);

		if (!m_native) Log::error("Win32 window not created");

		m_graphicsContext.reset(new Win32_OpenGL_GC(m_native));

		ShowWindow(m_native, SW_SHOW);
	}
	void Win32Window::close()
	{
		DestroyWindow(m_native);
	}
	void Win32Window::onUpdate(float timestep)
	{
		MSG msg = {};
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		m_graphicsContext->swapBuffers();
	}
	void Win32Window::onResize(unsigned int width, unsigned int height)
	{
	}
	void Win32Window::setVsync(bool vSync)
	{
	}
	void Win32Window::setEventCallback(const std::function<void(Event&)>& callback)
	{
	}
	LRESULT Win32Window::onWin32Msg(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{

		//case WM_DESTROY:
		//	std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
		//	WindowCloseEvent event;
		//	callback(event);
		//	break;

		//case WM_SIZE:
		//{
		//	int32_t width = LOWORD(lParam);
		//	int32_t height = HIWORD(wParam);
		//	std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
		//	WindowResizeEvent event(width, height);
		//	callback(event);
		//	

		//}
		//case WM_KEYDOWN :
		//{
		//	int32_t keyCode = static_cast<int32_t>(wParam);
		//	int32_t repeatCount = LOWORD(lParam);
		//}
		default:
			DefWindowProc(hWin, msg, wParam, lParam);
			break;
		}
		return 0;
	}
}