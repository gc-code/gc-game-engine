#ifndef __IMAGE_H__
#define __IMAGE_H__

namespace Renderer
{
	class Image
	{
	public:

		Image();

		virtual ~Image();

		enum PixelFormat
		{
			PIXEL_R8G8B8,
			PIXEL_R8G8B8A8,
		};

		virtual bool Load(const char* filePath) = 0;

		unsigned char* GetPixelData() { return mPixelData; }
		unsigned int GetPixelDataSize() { return mPixelDataSize; }

		unsigned int GetWidth() { return mImageWidth; }
		unsigned int GetHeight() { return mImageHeight; }

		unsigned int GetNumComponents() { return mNumComponents; }

		void Free();

	protected:

		PixelFormat mPixelFormat;

		unsigned int mImageWidth;
		unsigned int mImageHeight;

		unsigned int mNumComponents;

		unsigned int mPixelDataSize;
		unsigned char* mPixelData;
	};
}

#endif