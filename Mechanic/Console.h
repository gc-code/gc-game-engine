#ifndef __CONSOLE_H__
#define __CONSOLE_H__

namespace Mechanic
{
	class BasicWindow;

	class Console
	{
	public:

		Console();

		virtual ~Console();

		bool Init();

		void OutputText(const char* message);

		void ErrorSpin();

		void Update();

	private:

		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		char mConsoleBuffer[500];
		unsigned int mCurTextPos;

		HWND mWinTextArea;	// Native windows object pretty much unavoidable
		BasicWindow* mSysWindow;	// Native window object
	};
}

#endif