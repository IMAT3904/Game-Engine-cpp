/** \file renderAPI.cpp */

#include "engine_pch.h"
#include "rendering/RenderAPI.h"

#include "rendering/indexBuffer.h"
#include "rendering/shader.h"
#include "rendering/texture.h"
#include "rendering/vertexBuffer.h"
#include "rendering/vertexArray.h"

#include "systems/log.h"

#include "platform/OpenGL/OpenGLIndexBuffer.h"
#include "platform/OpenGL/OpenGLShader.h"
#include "platform/OpenGL/OpenGLTexture.h"
#include "platform/OpenGL/OpenGLVertexBuffer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

namespace Engine 
{
    RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

    IndexBuffer* IndexBuffer::create(uint32_t* indicies, uint32_t count)
    {
        switch (RenderAPI::getAPI())
        {
        case RenderAPI::API::None :
            Log::error("Not having a rendering API currently supported");
            break;
        case RenderAPI::API::OpenGL:
            return new OpenGLIndexBuffer(indicies, count);
            break;
            
        case RenderAPI::API::Diret3D:
            Log::error("Diret3D not currently supported");
            break;
        case RenderAPI::API::Vulkan:
            Log::error("Vulkan API not currently supported");
            break;
        }
        return nullptr;
    }

    Shader* Shader::create(const char* vertexFilePath, const char* fragmentFilePath)
    {
        switch (RenderAPI::getAPI())
        {
        case RenderAPI::API::None:
            Log::error("Not having a rendering API currently not supported");
            break;
        case RenderAPI::API::OpenGL:
            return new OpenGLShader(vertexFilePath,fragmentFilePath);
            break;

        case RenderAPI::API::Diret3D:
            Log::error("Diret3D not currently supported");
            break;
        case RenderAPI::API::Vulkan:
            Log::error("Vulkan API not currently supported");
            break;
        }
        return nullptr;
    }

    Shader* Shader::create(const char* filepath)
    {
        switch (RenderAPI::getAPI())
        {
        case RenderAPI::API::None:
            Log::error("Not having a rendering API currently not supported");
            break;
        case RenderAPI::API::OpenGL:
            return new OpenGLShader(filepath);
            break;

        case RenderAPI::API::Diret3D:
            Log::error("Diret3D not currently supported");
            break;
        case RenderAPI::API::Vulkan:
            Log::error("Vulkan API not currently supported");
            break;
        }
        return nullptr;
    }


    Texture* Texture::create(const char* filepath)
    {
        switch (RenderAPI::getAPI())
        {
        case RenderAPI::API::None:
            Log::error("Not having a rendering API currently not supported");
            break;
        case RenderAPI::API::OpenGL:
            return new OpenGLTexture(filepath);
            break;

        case RenderAPI::API::Diret3D:
            Log::error("Diret3D not currently supported");
            break;
        case RenderAPI::API::Vulkan:
            Log::error("Vulkan API not currently supported");
            break;
        }
        return nullptr;
    }
    Texture* Texture::create(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data)
    {
        switch (RenderAPI::getAPI())
        {
        case RenderAPI::API::None:
            Log::error("Not having a rendering API currently not supported");
            break;
        case RenderAPI::API::OpenGL:
            return new OpenGLTexture(width,height,channels,data);
            break;

        case RenderAPI::API::Diret3D:
            Log::error("Diret3D not currently supported");
            break;
        case RenderAPI::API::Vulkan:
            Log::error("Vulkan API not currently supported");
            break;
        }
        return nullptr;
    }

    VertexBuffer* VertexBuffer::create(void* vertices, uint32_t size, BufferLayout layout)
    {
        switch (RenderAPI::getAPI())
        {
        case RenderAPI::API::None:
            Log::error("Not having a rendering API currently not supported");
            break;
        case RenderAPI::API::OpenGL:
            return new OpenGLVertexBuffer(vertices, size, layout);
            break;

        case RenderAPI::API::Diret3D:
            Log::error("Diret3D not currently supported");
            break;
        case RenderAPI::API::Vulkan:
            Log::error("Vulkan API not currently supported");
            break;
        }
        return nullptr;
    }

    VertexArray* VertexArray::create()
    {
        switch (RenderAPI::getAPI())
        {
        case RenderAPI::API::None:
            Log::error("Not having a rendering API currently not supported");
            break;
        case RenderAPI::API::OpenGL:
            return new OpenGLVertexArray;
            break;

        case RenderAPI::API::Diret3D:
            Log::error("Diret3D not currently supported");
            break;
        case RenderAPI::API::Vulkan:
            Log::error("Vulkan API not currently supported");
            break;
        }
        return nullptr;
    }

}