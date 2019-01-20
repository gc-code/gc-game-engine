#include "Mouse.h"

namespace System
{
	Mouse::MouseButtons Mouse::mCurPressedButton = Mouse::MOUSE_NONE;
	unsigned int Mouse::mScreenCoordX = 0;
	unsigned int Mouse::mScreenCoordY = 0;

	Mouse::Mouse()
	{}

	Mouse::~Mouse()
	{}

	void Mouse::RegisterButtonPress(Mouse::MouseButtons buttonPressed)
	{
		mCurPressedButton = buttonPressed;
	}

	void Mouse::SetScreenCoordX(unsigned int x)
	{
		mScreenCoordX = x;
	}

	void Mouse::SetScreenCoordY(unsigned int y)
	{
		mScreenCoordY = y;
	}

	void Mouse::ClearState()
	{
		mCurPressedButton = MOUSE_NONE;
	}
}