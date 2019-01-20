#ifndef __GPUBUFFER_H__
#define __GPUBUFFER_H__

/*
	Container for the state of a buffer object.
*/

namespace Renderer
{
	class GPUBuffer
	{
	public:

		GPUBuffer();

		virtual ~GPUBuffer();

		enum BufferType
		{
			VERTEX_BUFFER,
			INDEX_BUFFER,
			CONSTANT_BUFFER,
		};

		enum BufferAccess
		{
			ACCESS_IMMUTABLE,
			ACCESS_STATIC,
			ACCESS_DYNAMIC,
		};

		void SetBufferType(BufferType bufferType) { mBufferType = bufferType; }
		void SetBufferAccess(BufferAccess bufferAccess) { mBufferAccess = bufferAccess; }
		void SetD3DBuffer(ID3D11Buffer* d3dBuffer) { mD3DBuffer = d3dBuffer; }
		void SetDataPointer(void* dataPtr) { mPreviousData = dataPtr; }
		void SetBufferSize(unsigned int size) { mBufferSize = size; }

		BufferType GetBufferType() { return mBufferType; }
		BufferAccess GetBufferAccess() { return mBufferAccess; }
		ID3D11Buffer* GetD3DBuffer() { return mD3DBuffer; }
		void* GetPrevDataPointer() { return mPreviousData; }
		unsigned int GetDataSize() { return mBufferSize; }

		void Free();

	private:

		BufferType mBufferType;
		BufferAccess mBufferAccess;

		void* mPreviousData;
		unsigned int mBufferSize;

		ID3D11Buffer* mD3DBuffer;
	};
}

#endif