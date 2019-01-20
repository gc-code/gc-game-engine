#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

namespace System
{
	class Keyboard
	{
	public:

		Keyboard();

		virtual ~Keyboard();

		enum Keys
		{
			KEY_NONE,
			KEY_ESC,

			KEY_UP,
			KEY_DOWN,
			KEY_LEFT,
			KEY_RIGHT,

			KEY_W,
			KEY_A,
			KEY_S,
			KEY_D,
		};

		static void RegisterKeyEvent(Keys pressedKey) { mCurrentKey = pressedKey; }

		bool IsKeyPressed(Keys checkKey) 
		{ 
			if (mCurrentKey == checkKey) return true;
			return false;
		}

		static void ClearState() { mCurrentKey = KEY_NONE; }

	private:

		static Keys mCurrentKey;
	};
}

#endif