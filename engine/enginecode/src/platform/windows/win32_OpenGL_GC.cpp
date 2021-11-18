/* \file win32_OpenGL_GC.cpp*/
#include "engine_pch.h"
#include "platform/windows/win32_OpenGL_GC.h"
#include "systems/log.h"
#include <glad/glad.h>

namespace Engine
{
	Win32_OpenGL_GC::Win32_OpenGL_GC(HWND hWin) : m_window(hWin)
	{
		init();
	}
	Win32_OpenGL_GC::~Win32_OpenGL_GC()
	{
		wglMakeCurrent(0, 0);
		wglDeleteContext(m_context);
		ReleaseDC(m_window, m_deviceContext);
	}
	void Win32_OpenGL_GC::init()
	{
		m_deviceContext = GetDC(m_window);
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		int format = ChoosePixelFormat(m_deviceContext, &pfd);
		if (!SetPixelFormat(m_deviceContext, format, &pfd)) Log::error("Could not set pixel format");
		
		m_context = wglCreateContext(m_deviceContext);
		if (!wglMakeCurrent(m_deviceContext, m_context)) Log::error("Could not set wGL context to current");

		if (!gladLoadGL()) Log::error("Could not load Glad");
	}
	void Win32_OpenGL_GC::swapBuffers()
	{
		SwapBuffers(m_deviceContext);
	}
}
