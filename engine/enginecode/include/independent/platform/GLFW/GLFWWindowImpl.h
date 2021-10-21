#pragma once
/** \file GLFWWindowImpl.h */

#include "core/window.h"
#include <GLFW/glfw3.h>
#include "platform/GLFW/GLFW_OpenGL_GC.h"

namespace Engine
{
	/* \class GLFWWindowImpl
	* Implementation of a window using GLFW
	*/

	class GLFWWindowImpl : public Window
	{
	public:
		GLFWWindowImpl(const WindowProperties& properties); //!< Constructor
		virtual void init(const WindowProperties& properties) override; //!< Initialise the window
		virtual void close() override; //!< Close the window
		//virtual ~Window() {};
		virtual void onUpdate(float timestep) override;
		virtual void onResize(unsigned int width, unsigned int height) override;
		virtual void setVsync(bool vSync) override;
		virtual void setEventCallback(const std::function<void(Event&)>& callback) override;
		virtual inline unsigned int getWidth() const override { return m_props.height; };
		virtual inline unsigned int getHeight() const override { return m_props.width; };
		virtual inline void* getNativeWindow() const  override { return m_native; };
		virtual inline bool isFullScreenMode() const  override { return m_props.isFullScreen; };
		virtual inline bool isVSync() const override { return m_props.isVsync; };

	private:
		std::function<void(Event&)> m_callback; //General OnEvent call back - bout to application::OnEvent(e)
		WindowProperties m_props;
		GLFWwindow* m_native;
		float m_aspectRatio;
		std::shared_ptr<GraphicsContext> m_graphicsContext;
	};

}

