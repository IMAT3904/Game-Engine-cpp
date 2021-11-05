#pragma once
/** \file RenderAPI.h */

namespace Engine
{
	class RenderAPI 
	{
	public:
		enum class API{None =0, OpenGL =1, Diret3D =2, Vulkan =3};
		static API getAPI() { return s_API; }
	private:
		static API s_API; //<The current API

	};
}