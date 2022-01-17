#include "eventTest.h"

TEST(Events, ResizeConcrutctor)
{
	int gotWidth = resizeEvent.getWidth();
	int gotHeight = resizeEvent.getHeight();
	Engine::EventType type = resizeEvent.getEventType();

	EXPECT_EQ(gotWidth, width);
	EXPECT_EQ(gotHeight, height);
}

TEST(Events, DispatchHandleTrue)
{
	//Dispatcher dispatch events?
	Engine::EventDispatcher dispatcher(resizeEvent);
	dispatcher.dispatch<Engine::WindowResizeEvent>(std::bind(OnResizeTrue, std::placeholders::_1));
	bool result = resizeEvent.isHandled();
	EXPECT_TRUE(result);
}

TEST(Events, KeyPressed)
{
	int keycode = 32;
	EXPECT_EQ(keycode, keyTypedEvent.getKeyCode());
}
TEST(Events, WindowCloseEventInRightCategory)
{
	bool currentCategory = closeEvent.isInCategory(Engine::EventCategory::EventCategoryWindow);
	EXPECT_TRUE(currentCategory);
}
TEST(Events, WindowMoveEvent)
{
	//New cordinates are reported correctly?
	int newX = 200, newY = 200;
	
	EXPECT_EQ(newX, windowMoveEvent.getxPos());
	EXPECT_EQ(newY, windowMoveEvent.getyPos());
}
TEST(Events, mouseButtonPressedEvent)
{
	EXPECT_EQ(mouseButtonPressedEvent.getButton(),NG_MOUSE_BUTTON_1);
}