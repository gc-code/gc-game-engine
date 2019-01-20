#include "Renderer.h"

namespace Renderer
{
	PixelShader::PixelShader()
		: mD3DPixelShader(NULL)
	{}

	PixelShader::~PixelShader()
	{
		Free();
	}

	void PixelShader::Free()
	{
		if (mD3DPixelShader) mD3DPixelShader->Release();

		mD3DPixelShader = NULL;
	}
}