#pragma once
/** \file camera.h */

#include <glm/gtc/matrix_transform.hpp>

#include "systems/event.h"
namespace Engine
{
	/**
	\class FollowCamera
	\brief Another camera class wich follows object.
	*/
	class FollowCamera
	{
	public:
		FollowCamera(int width, int height, glm::mat4* objectTransform); //!< Constructor.
		glm::vec3 position; //!< CameraPosition.
		glm::vec3 m_offset = glm::vec3(0, 1.3f, -4); //!< Offset from the target.
		glm::mat4 view; //!< ViewMatrix.
		glm::mat4 projection; //!< Projection Matrix.

		void Input(float deltaTime); //!< Input Logic.
	private:
		glm::mat4* objectTransform; //!< Pointer to object transform.
		void UpdateViewAndCameraPos(); //!< Update view and camera position.
		void CalculateProjection(float FOVdeg, float nearPlane, float farPlane); //!< Calculate Projection matrix.
		int m_screenWidth; //!< Screen width.
		int m_screenHeight; //!< Screen height.
	};
}