/* \GLFWWindowImpl.cpp */

#include "engine_pch.h"
#include "platform/GLFW/GLFWWindowImpl.h"
#include "systems/log.h"

namespace Engine 
{
#ifdef NG_PLATFORM_WINDOWS
	Window* Window::create(const WindowProperties& properties)
	{
		return new GLFWWindowImpl(properties);
	}

#endif

	
	GLFWWindowImpl::GLFWWindowImpl(const WindowProperties& properties)
	{
		init(properties);
	}

	void GLFWWindowImpl::init(const WindowProperties& properties)
	{
		m_props = properties;

		m_aspectRatio = static_cast<float>(m_props.width) / static_cast<float>(m_props.height);

		if (m_props.isFullScreen)
		{
			Log::info("Fullscreen not yet implemented");

			glfwSetWindowCloseCallback(m_native,
				[](GLFWwindow* window)
			{
				std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				callback(event);
			}
			);
		}
		else
		{
			m_native = glfwCreateWindow(m_props.width, m_props.height, m_props.title, nullptr, nullptr);
			glfwSetWindowUserPointer(m_native, &m_callback);

		}
		m_graphicsContext.reset(new GLFW_OpenGL_GC(m_native));
		m_graphicsContext->init();


	}
	void GLFWWindowImpl::close()
	{
		glfwDestroyWindow(m_native);
	}
	void GLFWWindowImpl::onUpdate(float timestep)
	{
		glfwPollEvents();
		m_graphicsContext->swapBuffers();
	}
	void GLFWWindowImpl::onResize(unsigned int width, unsigned int height)
	{
	}
	void GLFWWindowImpl::setVsync(bool vSync)
	{
		m_props.isVsync = vSync;
		if (m_props.isVsync)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
	}
	void GLFWWindowImpl::setEventCallback(const std::function<void(Event&)>& callback)
	{
	}
}