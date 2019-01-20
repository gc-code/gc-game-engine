#include <Windows.h>
#include "BasicWindow.h"

namespace Mechanic
{
	BasicWindow::BasicWindow()
		: mWidth(0)
		, mHeight(0)
	{}

	BasicWindow::BasicWindow(const char* title, const char* className, unsigned int width, unsigned int height, 
		WNDPROC wndProc)
	{
		Init(title, className, width, height, wndProc);
	}

	BasicWindow::~BasicWindow()
	{
		Destroy();
	}

	bool BasicWindow::Init(const char* title, const char* className, unsigned int width, unsigned int height,
		WNDPROC wndProc)
	{
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = wndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = mHInstance;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = className;
		wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			return false;
		}

		mHWnd = CreateWindowEx(WS_EX_CLIENTEDGE,
			className,
			title,
			WS_OVERLAPPED | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			NULL, NULL, mHInstance, NULL);

		if (mHWnd == NULL)
			return false;

		ShowWindow(mHWnd, 1);

		return true;
	}

	HWND BasicWindow::CreateTextArea(unsigned int x, unsigned int y,
		unsigned int width, unsigned int height)
	{
		HWND textArea = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), NULL, WS_CHILD | WS_DISABLED | WS_VISIBLE
			| CS_VREDRAW | CS_HREDRAW | ES_MULTILINE | ES_WANTRETURN,
			x, y, width, height, mHWnd, NULL, mHInstance, NULL);

		return textArea;
	}

	void BasicWindow::UpdateText(HWND textArea, const char* text)
	{
		SetWindowText(textArea, text);
	}

	void BasicWindow::Spin()
	{
		MSG msg = { 0 };
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	void BasicWindow::DisplayWindow(HWND window)
	{
		ShowWindow(window, 1);
	}

	void BasicWindow::Update()
	{
		MSG msg;
		if (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void BasicWindow::Destroy()
	{
		DestroyWindow(mHWnd);
	}
}