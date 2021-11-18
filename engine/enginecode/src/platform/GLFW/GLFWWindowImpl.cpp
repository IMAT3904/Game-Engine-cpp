/* \GLFWWindowImpl.cpp */

#include "engine_pch.h"
#include "platform/GLFW/GLFWWindowImpl.h"
#include "systems/log.h"

namespace Engine 
{
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

			
		}
		else
		{
			m_native = glfwCreateWindow(m_props.width, m_props.height, m_props.title, nullptr, nullptr);
			glfwSetWindowUserPointer(m_native, &m_callback);

		}
		
		glfwSetWindowCloseCallback(m_native,
			[](GLFWwindow* window)
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			callback(event);
		}
		);

		glfwSetWindowSizeCallback(m_native,
			[](GLFWwindow* window, int width, int height)
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			WindowResizeEvent event(width,height);
			callback(event);
		}
		);

		glfwSetWindowFocusCallback(m_native,
			[](GLFWwindow* window, int windowFocused)
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			if (windowFocused == GLFW_TRUE)
			{
				WindowFocusEvent event;
				callback(event);
			}
			else
			{
				WindowLostFocusEvent event;
				callback(event);
			}
		}
			);

		glfwSetWindowPosCallback(m_native,
			[](GLFWwindow* window, int newX,int newY)
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			WindowMovedEvent event(newX,newY);
			callback(event);
		}
			
			);

		glfwSetCharCallback(m_native,
			[](GLFWwindow* window, unsigned int codePoint)
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(codePoint);
			callback(event);
		}

		);



		glfwSetKeyCallback(m_native,
			[](GLFWwindow* window, int keycode, int scancode, int action, int mods)
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			if (action == GLFW_PRESS)
			{
				KeyPressedEvent event(keycode,0);
				callback(event);
			}
			else if (action == GLFW_REPEAT)
			{
				KeyPressedEvent event(keycode, 1);
				callback(event);
			}
			else if (action == GLFW_RELEASE)
			{
				KeyReleasedEvent event(keycode);
				callback(event);
			}
		}
		);
		m_graphicsContext.reset(new GLFW_OpenGL_GC(m_native));
		m_graphicsContext->init();


		glfwSetMouseButtonCallback(m_native,
			[](GLFWwindow* window, int mouseButton, int action, int mods)
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			if (action == GLFW_PRESS)
			{
				MouseButtonPressedEvent event(mouseButton);
				callback(event);
			}
			else if (action == GLFW_RELEASE)
			{
				MouseButtonReleasedEvent event(mouseButton);
				callback(event);
			}
		}
			);

		glfwSetCursorPosCallback(m_native,
			[](GLFWwindow* window, double xPos, double yPos)
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event(xPos, yPos);
			callback(event);
		}

			);

		glfwSetScrollCallback(m_native,
			[](GLFWwindow* window, double xPos, double yPos)
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event(xPos, yPos);
			callback(event);

		}
			);

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
		m_callback = callback;
	}
}