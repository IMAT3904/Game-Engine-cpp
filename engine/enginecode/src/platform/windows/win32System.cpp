/* \file win32System.cpp*/

#include "engine_pch.h"

#include "platform/windows/win32System.h"
#include "platform/windows/win32Window.h"


namespace Engine 
{
	void Win32System::start(SystemSignal init, ...)
	{
		WNDCLASSEX wndClass;

		memset(&wndClass, 0, sizeof(WNDCLASSEX));
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.hInstance = GetModuleHandle(NULL);
		wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
		wndClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(0, IDC_ARROW);
		wndClass.lpszClassName = L"NGWin32Window";
		wndClass.lpfnWndProc = [](HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT CALLBACK
		{
			auto winPtr = Application::getInstance().getWindow();
			if (winPtr)
			{
				auto win = static_cast<Win32Window*>(winPtr.get());
				win->onWin32Msg(hWin, msg, wParam, lParam);
			}
			else
			{
				return DefWindowProc(hWin, msg, wParam, lParam);
			}
		};
		if (!RegisterClassEx(&wndClass)) Log::error("Could not register win32 class");

	}
;	void Win32System::stop(SystemSignal close, ...)
	{
	if (!UnregisterClass(L"NGWin32Window", GetModuleHandle(NULL))) Log::error("Could not register win32 class");
	}
}