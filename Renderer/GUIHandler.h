#ifndef __GUIHANDLER_H__
#define __GUIHANDLER_H__

#include <string>

namespace Renderer
{
	class GUIHandler
	{
	public:

		GUIHandler();
		virtual ~GUIHandler();

		static void ProcessFileString(const char* fileName);

		static void ProcessResizeEvent();

		bool FileEventReceived() { return mFileEvent; }
		bool ResizeEventReceived() { return mWindowResized; }

		std::string GetFileString();

	private:

		static std::string mFileString;
		static bool mWindowResized;
		static bool mFileEvent;
	};
}

#endif