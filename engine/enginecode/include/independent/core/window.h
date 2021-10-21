/* \file window.h */
#pragma once
#include <GLFW/glfw3.h>
#include <functional>

#include "systems/event.h"
#include "core/graphicsCotext.h"
namespace Engine
{

	/** \class Window
	* Abstract windows base class.
	*/

	struct WindowProperties
	{
		char* title;
		uint32_t  width;
		uint32_t height;
		bool isFullScreen;
		bool isVsync;

		WindowProperties(char* title = "My window", unsigned int width = 800, unsigned int height = 600, bool fullscreen = false, bool vsync = false) : title(title), width(width), height(height),isVsync(vsync), isFullScreen(fullscreen) {};
	};


	class Window
	{
	public:
		virtual void init(const WindowProperties& properties) = 0;
		virtual void close() = 0;
		virtual ~Window() {};
		virtual void onUpdate(float timestep) = 0;
		virtual void onResize(unsigned int width, unsigned int height) = 0;
		virtual void setVsync(bool vSync) = 0;
		virtual void setEventCallback(const std::function<void(Event&)>& callback) = 0;
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual void* getNativeWindow() const = 0;
		virtual bool isFullScreenMode() const = 0;
		virtual bool isVSync() const = 0;

		static Window* create(const WindowProperties& porperties = WindowProperties());
	protected:
		std::shared_ptr<GraphicsContext> m_context;

	};
}

