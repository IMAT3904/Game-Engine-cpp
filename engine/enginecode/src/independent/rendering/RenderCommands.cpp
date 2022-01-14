/* \file RenderCommands.cpp */

#include "engine_pch.h"
#include "rendering/RenderCommands.h"
#include "rendering/RenderAPI.h"
#include <glad/glad.h>

namespace Engine
{
	std::function<void(void)> RenderCommandFactory::clearDepthBufferCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Diret3D:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::OpenGL:
			return []() {glClear(GL_DEPTH_BUFFER_BIT); };
			break;
		default:
			return std::function<void(void)>();
			break;
		}
	}
	std::function<void(void)> RenderCommandFactory::clearColourBufferCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Diret3D:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::OpenGL:
			return []() {glClear(GL_COLOR_BUFFER_BIT); };
			break;
		default:
			return std::function<void(void)>();
			break;
		}
	}
	std::function<void(void)> RenderCommandFactory::enableDepthTestCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Diret3D:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::OpenGL:
			return []() {glEnable(GL_DEPTH_TEST); };
			break;
		default:
			return std::function<void(void)>();
			break;
		}
	}
	std::function<void(void)> RenderCommandFactory::disableDepthTestCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Diret3D:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::OpenGL:
			return []() {glDisable(GL_DEPTH_TEST); };
			break;
		default:
			return std::function<void(void)>();
			break;
		}
	}
	std::function<void(void)> RenderCommandFactory::enableBlendingCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Diret3D:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::OpenGL:
			return []() {glEnable(GL_BLEND); };
			break;
		default:
			return std::function<void(void)>();
			break;
		}
	}
	std::function<void(void)> RenderCommandFactory::getClearColourAndDepthBufferCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Diret3D:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::OpenGL:
			return []() {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };
			break;
		default:
			return std::function<void(void)>();
			break;
		}
		
	}
	std::function<void(void)> RenderCommandFactory::getsetColourCommand(float r, float g, float b, float a)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Diret3D:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
			break;
		case RenderAPI::API::OpenGL:
			return [r,g,b,a]() {glClearColor(r,g,b,a); };
			break;
		default:
			return std::function<void(void)>();
			break;
		}
	}
}