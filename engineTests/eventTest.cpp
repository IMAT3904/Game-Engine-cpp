#include "eventTest.h"

TEST(Events, ResizeConcrutctor)
{
	int gotWidth = re.getWidth();
	int gotHeight = re.getHeight();
	Engine::EventType type = re.getEventType();

	EXPECT_EQ(gotWidth, width);
	EXPECT_EQ(gotHeight, height);
}

TEST(Events, DispatchHandleTrue)
{
	Engine::EventDispatcher dispatcher(re);
	dispatcher.dispatch<Engine::WindowResizeEvent>(std::bind(OnResizeTrue, std::placeholders::_1));
	bool result = re.isHandled();
	EXPECT_TRUE(result);
}

TEST(Events, KeyPressed)
{
	int keycode = 32;
	EXPECT_EQ(keycode, ke.getKeyCode());
}