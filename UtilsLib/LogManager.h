#ifndef __LOGMANAGER_H__
#define __LOGMANAGER_H__

#include <fstream>

namespace UtilsLib
{
	class LogManager
	{
	public:

		LogManager();

		virtual ~LogManager();

		static void InitLog(const char* filePath);

		static void Log(const char* msg);

		static void CloseLog();

	private:

		static std::ofstream mLogFile;
	};
}

#endif