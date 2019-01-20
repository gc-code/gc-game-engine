#include "Renderer.h"

namespace Renderer
{
	Texture2D::Texture2D()
		: mD3DShaderResourceView(NULL)
	{}

	Texture2D::~Texture2D()
	{
		Free();
	}

	void Texture2D::Free()
	{
		if (mD3DShaderResourceView) mD3DShaderResourceView->Release();

		mD3DShaderResourceView = NULL;
	}
}