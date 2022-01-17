#pragma once
#include <gtest/gtest.h>

#include "systems/event.h"
#include "systems/eventdispatcher.h"
#include "systems/GLFWCodes.h"

const int width = 1024;
const int height = 720;

Engine::WindowResizeEvent resizeEvent(width, height);
Engine::WindowCloseEvent closeEvent;
Engine::KeyTypedEvent keyTypedEvent(32);
Engine::WindowMovedEvent windowMoveEvent(200,200);
Engine::MouseButtonPressedEvent mouseButtonPressedEvent(NG_MOUSE_BUTTON_1);

bool OnResizeTrue(Engine::WindowResizeEvent& e)
{
	return true;
}

bool OnResizeFalse(Engine::WindowResizeEvent& e)
{
	return false;
}