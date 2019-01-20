#ifndef __BASICWINDOW_H__
#define __BASICWINDOW_H__

namespace Mechanic
{
	class BasicWindow
	{
	public:

		BasicWindow();
		BasicWindow(const char* title, const char* className, unsigned int width, unsigned int height, WNDPROC wndProc);

		virtual ~BasicWindow();

		bool Init(const char* title, const char* className, unsigned int width, unsigned int height,
			WNDPROC wndProc);

		HWND CreateTextArea(unsigned int x, unsigned int y,
			unsigned int width, unsigned int height);	// Text area treated as window object

		void UpdateText(HWND textArea, const char* text);

		void DisplayWindow(HWND window);

		void Spin();

		void Update();

		void Destroy();

	private:

		unsigned int mWidth;
		unsigned int mHeight;

		unsigned int mTextAreaBackground[4];

		HINSTANCE mHInstance;
		HWND mHWnd;
	};
}

#endif