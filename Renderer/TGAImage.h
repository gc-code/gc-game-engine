#ifndef __TGAIMAGE_H__
#define __TGAIMAGE_H__

namespace Renderer
{
	class TGAImage : public Image
	{
	public:

		TGAImage();

		virtual ~TGAImage();

		struct TGAHeader
		{
			unsigned char IdLength;
			unsigned char ColourMapType;
			unsigned char ImageType;

			unsigned short FirstEntryIndex;
			unsigned short ColourMapLength;
			unsigned char ColourMapEntrySize;

			unsigned short XOrigin;
			unsigned short YOrigin;
			unsigned short ImageWidth;
			unsigned short ImageHeight;
			unsigned char PixelDepth;
			unsigned char ImageDescriptor;
		};

		bool Load(const char* filePath);

	private:

		TGAHeader* mFileHeader;
	};
}

#endif