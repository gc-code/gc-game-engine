#include <fstream>
#include "BinaryBuffer.h"

namespace Utils
{
	BinaryBuffer::BinaryBuffer()
		: mBuffer(0)
		, mBufferSize(0)
	{}

	BinaryBuffer::BinaryBuffer(const char* filePath)
		: mBuffer(0)
		, mBufferSize(0)
	{
		ReadFromFile(filePath);
	}

	BinaryBuffer::~BinaryBuffer()
	{
		Free();
	}

	bool BinaryBuffer::ReadFromFile(const char* filePath)
	{
		std::ifstream ifs(filePath, std::ios::in | std::ios::binary);
		if (!ifs.is_open())
			return false;

		ifs.seekg(0, std::ios::end);
		mBufferSize = (unsigned int)ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		mBuffer = new unsigned char[mBufferSize];
		ifs.read((char*)mBuffer, mBufferSize);

		mBufferPtr = mBuffer;

		ifs.close();

		return true;
	}

	bool BinaryBuffer::WriteToFile(const char* filePath)
	{
		std::ofstream ofs(filePath, std::ios::out | std::ios::binary);
		if (!ofs.is_open())
			return false;

		ofs.write((const char*)mBuffer, mBufferSize);

		ofs.close();

		return true;
	}

	unsigned char BinaryBuffer::ReadLittleByte()
	{
		unsigned char outByte;
		outByte = *mBufferPtr;
		mBufferPtr++;
		return outByte;
	}

	unsigned short BinaryBuffer::ReadLittleWord()
	{
		unsigned short outWord;
		outWord = (*(unsigned short*)mBufferPtr);
		mBufferPtr += 2;
		return outWord;
	}

	unsigned int BinaryBuffer::ReadLittleDWord()
	{
		unsigned int outDWord;
		outDWord = (*(unsigned int*)mBufferPtr);
		mBufferPtr += 4;
		return outDWord;
	}

	float BinaryBuffer::ReadLittleFloat()
	{
		float outFloat;
		outFloat = (*(float*)mBufferPtr);
		mBufferPtr += 4;
		return outFloat;
	}

	short BinaryBuffer::ReadLittleShort()
	{
		short outShort;
		outShort = (*(short*)mBufferPtr);
		mBufferPtr += 2;
		return outShort;
	}

	void BinaryBuffer::ReadString(char* strBuf, unsigned int bufSize)
	{
		char tempChar;
		unsigned int i = 0;
		for (;;)
		{
			tempChar = *mBufferPtr;
			mBufferPtr++;

			*strBuf++ = tempChar;
			if (tempChar == '\0')
				break;

			i++;
			if (i >= bufSize)
				break;
		}
	}

	void BinaryBuffer::AllocateFileBuffer(unsigned int size)
	{
		mBuffer = new unsigned char[size];
		mBufferSize = size;
		mBufferPtr = mBuffer;
	}

	void BinaryBuffer::WriteLittleByte(unsigned char outByte)
	{
		*mBufferPtr = outByte;
		mBufferPtr++;
	}

	void BinaryBuffer::WriteLittleWord(unsigned short outWord)
	{
		*(unsigned short*)mBufferPtr = outWord;
		mBufferPtr += 2;
	}

	void BinaryBuffer::WriteLittleDWord(unsigned int outDWord)
	{
		*(unsigned int*)mBufferPtr = outDWord;
		mBufferPtr += 4;
	}

	void BinaryBuffer::WriteLittleByteArray(unsigned char* outByteArray, unsigned int size)
	{
		memcpy(mBufferPtr, outByteArray, size);
		mBufferPtr += size;
	}

	void BinaryBuffer::Free()
	{
		if (mBuffer) delete[] mBuffer;
		
		mBuffer = 0;
		mBufferSize = 0;
	}
}