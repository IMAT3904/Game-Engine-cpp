/* \file RenderCommands.cpp */
#include "engine_pch.h"
#include <glad/glad.h>
#include "rendering/camera.h"
#include "glm/gtx/vector_angle.hpp"
#include "independent/core/inputPoller.h"
#include "independent/systems/GLFWCodes.h"
#include "systems/log.h"
 
namespace Engine
{
	Camera::Camera(int width, int height, glm::vec3 position) 
	{
		Camera::width = width;
		Camera::height = height;
		Camera::position = position;
		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);
		firstClick = false;
		CalculateProjection(60.0f, 1.f, 100.f);
	}
	void Camera::CalculateProjection(float FOVdeg, float nearPlane, float farPlane)
	{
		projection = glm::perspective(glm::radians(FOVdeg), float(width / height), nearPlane, farPlane);
	}
	void Camera::CalculateViewMatrix()
	{
		view = glm::lookAt(position, position + orientation, upVector);
	}
	void Camera::Input(float deltaTime)
	{
		CalculateViewMatrix();
		if (InputPoller::isKeyPressed(NG_KEY_W))
		{
			position += speed * orientation * deltaTime;
		}
		if (InputPoller::isKeyPressed(NG_KEY_S))
		{
			position += speed * -orientation * deltaTime;
		}
		if (InputPoller::isKeyPressed(NG_KEY_A))
		{
			position += speed * -glm::normalize(glm::cross(orientation,upVector)) * deltaTime;
		}
		if (InputPoller::isKeyPressed(NG_KEY_D))
		{
			position += speed * glm::normalize(glm::cross(orientation, upVector)) * deltaTime;
		}
		if (InputPoller::isKeyPressed(NG_KEY_SPACE))
		{
			position += speed * upVector * deltaTime;
		}
		if (InputPoller::isKeyPressed(NG_KEY_LEFT_CONTROL))
		{
			position += speed * -upVector * deltaTime;
		}

		if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_1))
		{
			if (!firstClick)
			{
				lastMouseX = InputPoller::getMouseX();
				lastMouseY = InputPoller::getMouseY();
				firstClick = true;
			}



			float mouseX = InputPoller::getMouseX();
			float mouseY = InputPoller::getMouseY();

			float deltaX = (mouseX - lastMouseX)*mouseSensitivity * deltaTime;
			float deltaY = (mouseY - lastMouseY) *mouseSensitivity * deltaTime;

			lastMouseX = mouseX;
			lastMouseY = mouseY;

			

			if (deltaY != 0)
			{
				glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-deltaY), glm::normalize(glm::cross(orientation, upVector)));
				if (!((glm::angle(newOrientation, upVector) <= glm::radians(5.0f) || glm::angle(newOrientation, -upVector) <= glm::radians(5.0f))))
					orientation = newOrientation;
			}
			if(deltaX != 0)
			orientation = glm::rotate(orientation, glm::radians(-deltaX), upVector);
		}
		else
		{
			firstClick = false;
		}


	}
}