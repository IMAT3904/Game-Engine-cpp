#pragma once
/** \file camera.h */

#include <glm/gtc/matrix_transform.hpp>

#include "systems/event.h"
namespace Engine
{
	/**
	\class Camera
	\brief FreeLook Camera class that allows to fly and look around the scene.
	*/
	class Camera
	{
	public:
		Camera(int width, int height, glm::vec3 position); //!< Constructor.

		glm::vec3 position; //!< Camera positon on the scene.
		glm::mat4 view; //!< View matrix.
		glm::mat4 projection; //!< Projection matrix.

		void CalculateProjection(float FOVdeg, float nearPlane, float farPlane); //!< Calculates projection matrix.
		void Input(float deltaTime); //!< Procecsses all keyaboard and mouse camera movement.

	private:
		void CalculateViewMatrix(); //!< Calculates View Matrix
		glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -0.5f); //!< Camera orientation
		glm::vec3 upVector = glm::vec3(0.0f, 1.f, 0.f); //!< up vector
		float lastMouseX; //!< Last mouse x positon.
		float lastMouseY; //!< Last mous y position.
		bool firstClick; //!< Is camera mouse being tracked?
		int width; //!< screen width.
		int height; //!< screen height.
		float speed = 5.f; //!< Movement speed.
		float mouseSensitivity = 25.f; //!< Mouse sensitivity.
	};
}