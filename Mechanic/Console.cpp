#include <Windows.h>
#include "Win32/BasicWindow.h"
#include "Console.h"

namespace Mechanic
{
	Console::Console()
	{}

	Console::~Console()
	{}

	bool Console::Init()
	{
		mSysWindow = new BasicWindow();
		if (!mSysWindow->Init("Output Console", "Console", 550, 500, WindowProc))
			return false;

		mWinTextArea = mSysWindow->CreateTextArea(15, 20, 500, 400);
		mCurTextPos = 0;

		return true;
	}

	void Console::ErrorSpin()
	{
		mSysWindow->Spin();
	}

	void Console::OutputText(const char* message)
	{
		int stringLen = strlen(message);
		if ((mCurTextPos + stringLen) > 500)
			mCurTextPos = 0;

		strcpy(mConsoleBuffer + mCurTextPos, message);
		mSysWindow->UpdateText(mWinTextArea, mConsoleBuffer);
		mCurTextPos += stringLen;
	}

	void Console::Update()
	{
		mSysWindow->Update();
	}

	LRESULT CALLBACK Console::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		
		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
}