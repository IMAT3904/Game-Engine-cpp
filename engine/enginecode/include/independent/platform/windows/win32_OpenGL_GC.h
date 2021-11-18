/* \file win32_OpenGL_GC.h*/
#pragma once

#include "core/graphicsCotext.h"


// Targeting Windows 10 only

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <windows.h>
namespace Engine 
{
	class Win32_OpenGL_GC : public GraphicsContext
	{
	public:
		Win32_OpenGL_GC(HWND hWin); //!< Constructor which takes a window handle
		~Win32_OpenGL_GC(); //!< Destructor
		virtual void init() override; //!< Init the graphics context for the given Windowing API
		virtual void swapBuffers() override; //!< Swap the front and back buffer (double buffering)
	private:
		HWND m_window = nullptr; //!< Window handle
		HDC m_deviceContext = nullptr; //!< Device context handle
		HGLRC m_context = nullptr; //!< OpenGL contex handle
	};
}
