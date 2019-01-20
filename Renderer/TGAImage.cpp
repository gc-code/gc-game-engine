#include "BinaryBuffer.h"
#include "Renderer.h"

using namespace Utils;

namespace Renderer
{
	TGAImage::TGAImage()
		: mFileHeader(0)
	{}

	TGAImage::~TGAImage()
	{
		if (mFileHeader) delete mFileHeader;
	}

	bool TGAImage::Load(const char* filePath)
	{
		BinaryBuffer* binaryBuffer = new BinaryBuffer();
		if (!binaryBuffer->ReadFromFile(filePath))
			return false;

		mFileHeader = new TGAHeader();

		mFileHeader->IdLength = binaryBuffer->ReadLittleByte();
		mFileHeader->ColourMapType = binaryBuffer->ReadLittleByte();
		mFileHeader->ImageType = binaryBuffer->ReadLittleByte();

		mFileHeader->FirstEntryIndex = binaryBuffer->ReadLittleWord();
		mFileHeader->ColourMapLength = binaryBuffer->ReadLittleWord();
		mFileHeader->ColourMapEntrySize = binaryBuffer->ReadLittleByte();

		mFileHeader->XOrigin = binaryBuffer->ReadLittleWord();
		mFileHeader->YOrigin = binaryBuffer->ReadLittleWord();
		mFileHeader->ImageWidth = binaryBuffer->ReadLittleWord();
		mFileHeader->ImageHeight = binaryBuffer->ReadLittleWord();
		mFileHeader->PixelDepth = binaryBuffer->ReadLittleByte();
		mFileHeader->ImageDescriptor = binaryBuffer->ReadLittleByte();

		mPixelDataSize = mFileHeader->ImageWidth * mFileHeader->ImageHeight * 4;
		mPixelData = new unsigned char[mPixelDataSize];
		mImageWidth = mFileHeader->ImageWidth;
		mImageHeight = mFileHeader->ImageHeight;
		mNumComponents = 4;

		unsigned char* pixelBufferPtr;
		if (mFileHeader->ImageType == 2 || mFileHeader->ImageType == 3)
		{
			for (int i = mFileHeader->ImageWidth - 1; i >= 0; i--)
			{
				pixelBufferPtr = mPixelData + i*mFileHeader->ImageHeight * 4;
				for (int j = 0; j < mFileHeader->ImageHeight; j++)
				{
					unsigned char red;
					unsigned char green;
					unsigned char blue;
					unsigned char alpha;
					switch (mFileHeader->PixelDepth)
					{
					case 8:
						red = binaryBuffer->ReadLittleByte();
						green = red;
						blue = red;

						*pixelBufferPtr++ = blue;
						*pixelBufferPtr++ = green;
						*pixelBufferPtr++ = red;
						*pixelBufferPtr++ = 255;
						break;

					case 24:
						red = binaryBuffer->ReadLittleByte();
						green = binaryBuffer->ReadLittleByte();
						blue = binaryBuffer->ReadLittleByte();
						
						*pixelBufferPtr++ = blue;
						*pixelBufferPtr++ = green;
						*pixelBufferPtr++ = red;
						*pixelBufferPtr++ = 255;
						break;

					case 32:
						red = binaryBuffer->ReadLittleByte();
						green = binaryBuffer->ReadLittleByte();
						blue = binaryBuffer->ReadLittleByte();
						alpha = binaryBuffer->ReadLittleByte();

						*pixelBufferPtr++ = blue;
						*pixelBufferPtr++ = green;
						*pixelBufferPtr++ = red;
						*pixelBufferPtr++ = alpha;
						break;

					default:
						return false;
					}
				}
			}
		}

		delete binaryBuffer;
		return true;
	}
}