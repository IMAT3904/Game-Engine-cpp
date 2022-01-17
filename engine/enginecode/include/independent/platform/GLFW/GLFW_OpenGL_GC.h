/** \file GLFW_OpenGL_GC.h */
#pragma once
#include "core/graphicsCotext.h"

namespace Engine
{
	/**
	\class GLFW_OpenGL_GC
	\brief Graphic context that uses GLFW implementation.
	*/
	class GLFW_OpenGL_GC : public GraphicsContext
	{
	public:
		GLFW_OpenGL_GC(GLFWwindow* win) : m_window(win) {} //!< Constructor.
		virtual void init() override; //!< Init the graphics context for the given Windowing API.
		virtual void swapBuffers() override; //!< Swap the front and back buffer (double buffering).

	private:
		GLFWwindow* m_window; //!< Pointer to GLFW window.
	};
}