#include "ErrorHandler.h"

namespace Utils
{
	ErrorHandler::ErrorHandler()
	{}

	ErrorHandler::~ErrorHandler()
	{}

	void ErrorHandler::ReportError(ErrorCode error)
	{
		if (error == NULL)
		{
			ReportError(WARNING_ERRORHANDLER);
		}
		else if (error != NULL)
		{
			ErrortoStore(error);
		}
		else
		{
			ReportError(WARNING_ERRORHANDLER);
		}
		
	}

	void ErrorHandler::ErrortoStore(ErrorCode errorcode)
	{
		ErrorStore.push_back(errorcode);
	}

	int ErrorHandler::GetError()
	{
		if (ErrorStore.empty())
		{
			currenterror = NO_ERROR;
			return currenterror;
		}
		else
		{
			currenterror = ErrorStore.back();
			return currenterror;
		}
		
	}
}