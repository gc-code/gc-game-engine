#include "GUIHandler.h"

namespace Renderer
{
	bool GUIHandler::mWindowResized = false;
	bool GUIHandler::mFileEvent = false;
	std::string GUIHandler::mFileString;

	GUIHandler::GUIHandler()
	{}

	GUIHandler::~GUIHandler()
	{}

	void GUIHandler::ProcessFileString(const char* fileName)
	{
		mFileString = fileName;
		mFileEvent = true;
	}

	void GUIHandler::ProcessResizeEvent()
	{
		mWindowResized = true;
	}

	std::string GUIHandler::GetFileString()
	{
		return mFileString;
	}
}