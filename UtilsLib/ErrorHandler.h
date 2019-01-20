#ifndef _ERRORHANDLER_H
#define _ERRORHANDLER_H

#include <vector>

namespace Utils
{
	class ErrorHandler
	{
	public:
		ErrorHandler();

		~ErrorHandler();

		//Error and Warning codes are only a base. Add more as per necessary.
		enum ErrorCode
		{
			ERROR_DIRECTX,
			ERROR_RENDER,
			ERROR_MEMORY,
			ERROR_ANIMATION,
			ERROR_INPUT,
			ERROR_ERRORHANDLER,
			NO_ERROR,
			WARNING_DIRECTX,
			WARNING_RENDER,
			WARNING_MEMORY,
			WARNING_ANIMATION,
			WARNING_INPUT,
			WARNING_ERRORHANDLER,
			NO_WARNING
		};

		static void ReportError(ErrorCode error);

		static int GetError();

	private:

		static void ErrortoStore(ErrorCode errorcode);

		static std::vector<ErrorCode>ErrorStore;

		static ErrorCode currenterror;		
	};
}


#endif