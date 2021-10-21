#pragma once
#include <gtest/gtest.h>

#include "systems/event.h"
#include "systems/eventdispatcher.h"

const int width = 1024;
const int height = 720;

Engine::WindowResizeEvent re(width, height);
Engine::WindowCloseEvent ce;
Engine::KeyTypedEvent ke(32);

bool OnResizeTrue(Engine::WindowResizeEvent& e)
{
	return true;
}

bool OnResizeFalse(Engine::WindowResizeEvent& e)
{
	return false;
}