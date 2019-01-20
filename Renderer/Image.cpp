#include "Renderer.h"

namespace Renderer
{
	Image::Image()
		: mPixelData(0)
		, mPixelDataSize(0)
	{}

	Image::~Image()
	{
		Free();
	}

	void Image::Free()
	{
		if (mPixelData) delete[] mPixelData;
		
		mPixelData = 0;
		mPixelDataSize = 0;
	}
}