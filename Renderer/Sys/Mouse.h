#ifndef __MOUSE_H__
#define __MOUSE_H__

namespace System
{
	class Mouse
	{
	public:
		enum MouseButtons
		{
			MOUSE_NONE,
			MOUSE_LEFT_CLICK,
			MOUSE_RIGHT_CLICK,
			MOUSE_MIDDLE_BUTTON,
			MOUSE_WHEEL,
		};

		Mouse();

		virtual ~Mouse();

		static void RegisterButtonPress(MouseButtons buttonPressed);

		static void SetScreenCoordX(unsigned int x);
		static void SetScreenCoordY(unsigned int y);

		static void ClearState();

	private:
		static MouseButtons mCurPressedButton;

		static unsigned int mScreenCoordX;
		static unsigned int mScreenCoordY;
	};
}

#endif