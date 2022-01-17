#include "engine_pch.h"
#include "rendering/followCamera.h"
#include "core/inputPoller.h"
#include "independent/systems/GLFWCodes.h"

namespace Engine
{
	FollowCamera::FollowCamera(int width, int height, glm::mat4* objectTransform)
	{
		FollowCamera::m_screenWidth = width;
		FollowCamera::m_screenHeight = height;
		FollowCamera::objectTransform = objectTransform;

		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);
		CalculateProjection(60, 0.1f, 100.f);
		UpdateViewAndCameraPos();
	}
	void FollowCamera::CalculateProjection(float FOVdeg, float nearPlane, float farPlane)
	{
		projection = glm::perspective(glm::radians(FOVdeg), float(m_screenWidth / m_screenHeight), nearPlane, farPlane);
	}
	void FollowCamera::Input(float deltaTime)
	{
		if (InputPoller::isKeyPressed(NG_KEY_UP))
		{
			(*objectTransform) = glm::translate((*objectTransform), glm::vec3(0, 0, -4.f)*deltaTime);
			UpdateViewAndCameraPos();
		}
		if (InputPoller::isKeyPressed(NG_KEY_LEFT))
		{
			(*objectTransform) = glm::rotate((*objectTransform), 2.2f * deltaTime,glm::vec3(0,1,0));
			UpdateViewAndCameraPos();
		}
		if (InputPoller::isKeyPressed(NG_KEY_RIGHT))
		{
			(*objectTransform) = glm::rotate((*objectTransform), -2.2f * deltaTime, glm::vec3(0, 1, 0));
			UpdateViewAndCameraPos();
		}
		if (InputPoller::isKeyPressed(NG_KEY_DOWN))
		{
			(*objectTransform) = glm::translate((*objectTransform), glm::vec3(0, 0, 4.f) * deltaTime);
			UpdateViewAndCameraPos();
		}
	}
	void FollowCamera::UpdateViewAndCameraPos()
	{
		glm::vec3 right = { (*objectTransform)[0][0],(*objectTransform)[0][1],(*objectTransform)[0][2] };
		glm::vec3 up = { (*objectTransform)[1][0],(*objectTransform)[1][1],(*objectTransform)[1][2] };
		glm::vec3 forward = { (*objectTransform)[2][0],(*objectTransform)[2][1],(*objectTransform)[2][2] };
		glm::vec3 objectPosition = { (*objectTransform)[3][0],(*objectTransform)[3][1],(*objectTransform)[3][2] };

		glm::vec3 posDelta(0.f);
		posDelta += forward * -m_offset.z;
		posDelta += right * m_offset.x;
		posDelta += up * m_offset.y;

		position = objectPosition + posDelta;
		glm::vec3 camForward = -posDelta;
		glm::vec3 camRight = glm::cross({ 0,1,0 }, posDelta);
		glm::vec3 camUp = glm::normalize(glm::cross(camRight, camForward));

		view = glm::lookAt(position, objectPosition, camUp);
	}
}