#include <fstream>
#include <assert.h>
#include "LogManager.h"

namespace UtilsLib
{
	LogManager::LogManager()
	{}

	LogManager::~LogManager()
	{}

	void LogManager::InitLog(const char* filePath)
	{
		mLogFile.open(filePath);
	}

	void LogManager::Log(const char* msg)
	{
		mLogFile << msg << std::endl;
	}

	void LogManager::CloseLog()
	{
		mLogFile.close();
	}
}