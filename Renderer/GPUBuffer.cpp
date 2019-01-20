#include "Renderer.h"

namespace Renderer
{
	GPUBuffer::GPUBuffer()
		: mBufferType(VERTEX_BUFFER)
		, mBufferAccess(ACCESS_STATIC)
		, mD3DBuffer(0)
	{}

	GPUBuffer::~GPUBuffer()
	{
		Free();
	}

	void GPUBuffer::Free()
	{
		if (mD3DBuffer) mD3DBuffer->Release();

		mD3DBuffer = 0;
	}
}