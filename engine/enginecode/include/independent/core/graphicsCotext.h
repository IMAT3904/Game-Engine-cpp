/** \file graphicsContext.h*/
#pragma once

namespace Engine
{
	class GraphicsContext
	{
	public:
		virtual void init() = 0; //!< Init the graphics context for the given Windowing API
		virtual void swapBuffers() = 0; //!< Swap the front and back buffer (double buffering)
	};
}
