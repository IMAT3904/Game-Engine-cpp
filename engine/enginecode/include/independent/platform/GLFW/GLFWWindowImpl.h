#pragma once
/** \file GLFWWindowImpl.h */

#include "core/window.h"
#include <GLFW/glfw3.h>
#include "platform/GLFW/GLFW_OpenGL_GC.h"

namespace Engine
{
	/* \class GLFWWindowImpl
	* \brief Implementation of a window using GLFW.
	*/

	class GLFWWindowImpl : public Window
	{
	public:
		GLFWWindowImpl(const WindowProperties& properties); //!< Constructor.
		virtual void init(const WindowProperties& properties) override; //!< Initialise the window.
		virtual void close() override; //!< Close the window.
		//virtual ~Window() {};
		virtual void onUpdate(float timestep) override; //!< Updates the logic while window is open.
		virtual void onResize(unsigned int width, unsigned int height) override; //!< Callback that fires  while window is being resized.
		virtual void setVsync(bool vSync) override; //!< Turns on or off vertical synchronization (v-sync)
		virtual void setEventCallback(const std::function<void(Event&)>& callback) override; //!< Set event callback.
		virtual inline unsigned int getWidth() const override { return m_props.width; }; //!< Get window width.
		virtual inline unsigned int getHeight() const override { return m_props.height; }; //!< Get window height.
		virtual inline void* getNativeWindow() const  override { return m_native; }; //!< Get window native size.
		virtual inline bool isFullScreenMode() const  override { return m_props.isFullScreen; }; //!< Is window in fullscreen mode?
		virtual inline bool isVSync() const override { return m_props.isVsync; }; //!< Is vertical synchronization (v-sync) enabled?

	private:
		std::function<void(Event&)> m_callback; //!<General OnEvent call back - bout to application::OnEvent(e)
		WindowProperties m_props; //!< holds all window properites.
		GLFWwindow* m_native; //!< Points to native window.
		float m_aspectRatio; //!< Holds information about proportions of the screen (aspect ratio).
		std::shared_ptr<GraphicsContext> m_graphicsContext; //!< Points to current graphic context.
	};

}

