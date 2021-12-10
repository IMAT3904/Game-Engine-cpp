#pragma once
/* \file RendererCommon.h */

#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>
#include "rendering/shaderDataType.h"
#include "rendering/texture.h"
#include "rendering/RendererCommon.h"
#include "rendering/shader.h"
#include "rendering/vertexArray.h"

using SceneWideUniforms = std::unordered_map<const char*, std::pair<Engine::ShaderDataType, void*>>;

inline static uint32_t pack(const glm::vec4& tint) { return 0; };