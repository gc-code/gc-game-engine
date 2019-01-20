#ifndef __BINARYBUFFER_H__
#define __BINARYBUFFER_H__

namespace Utils
{
	class BinaryBuffer
	{
	public:

		BinaryBuffer();
		BinaryBuffer(const char* filePath);

		virtual ~BinaryBuffer();

		bool ReadFromFile(const char* filePath);
		bool WriteToFile(const char* filePath);

		unsigned char ReadLittleByte();
		unsigned short ReadLittleWord();
		unsigned int ReadLittleDWord();
		void ReadString(char* strBuf, unsigned int bufSize);

		void AllocateFileBuffer(unsigned int size);
		void WriteLittleByte(unsigned char outByte);
		void WriteLittleWord(unsigned short outWord);
		void WriteLittleDWord(unsigned int outDWord);

		void WriteLittleByteArray(unsigned char* outByteArray, unsigned int size);

		short ReadLittleShort();

		float ReadLittleFloat();

		unsigned char* GetBuffer() { return mBuffer; }
		unsigned int GetBufferSize() { return mBufferSize; }

		void Free();

	private:

		unsigned char* mBuffer;
		unsigned char* mBufferPtr;
		unsigned int mBufferSize;
	};
}

#endif