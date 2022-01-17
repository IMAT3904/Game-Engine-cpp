#pragma once
/** \file RenderAPI.h */

namespace Engine
{
	/** \class RenderAPI
	* \brief Manages wchic Render API is activ;
	*/
	class RenderAPI 
	{
	public:
		enum class API{None =0, OpenGL =1, Diret3D =2, Vulkan =3}; //!< List of all supported and unsuported  APIs. 
		static API getAPI() { return s_API; } //!< Get current API
	private:
		static API s_API; //!<The current API

	};
}