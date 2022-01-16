#pragma once
/** \file camera.h */

#include <glm/gtc/matrix_transform.hpp>

#include "systems/event.h"
namespace Engine
{
	class Camera
	{
	public:
		Camera(int width, int height, glm::vec3 position);
		
		glm::vec3 position;
		glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -0.5f);
		glm::vec3 upVector = glm::vec3(0.0f, 1.f,0.f);

		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 transform;

		int width;
		int height;

		float speed = 5.f;
		float mouseSensitivity = 25.f;

		void Matrix(float FOVdeg, float nearPlane, float farPlane);

		void Input(float deltaTime);
	private:
		float lastMouseX;
		float lastMouseY;
		bool firstClick;
	};
}