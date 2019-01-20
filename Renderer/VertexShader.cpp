#include "Renderer.h"
#include "BinaryBuffer.h"

namespace Renderer
{
	VertexShader::VertexShader()
		: mD3DVertexShader(NULL)
		, mCodeBuffer(0)
		, mInputLayout(0)
		, mHasLevelCB(false)
		, mHasMaterialCB(false)
		, mHasModelCB(false)
		, mHasSkinningCB(false)
	{}

	VertexShader::~VertexShader()
	{
		Free();
	}

	bool VertexShader::HasInputLayout()
	{
		if (!mInputLayout)
			return false;

		return true;
	}

	void VertexShader::Free()
	{
		if (mD3DVertexShader) mD3DVertexShader->Release();
		if (mInputLayout) mInputLayout->Release();
		if (mCodeBuffer) delete mCodeBuffer;

		mD3DVertexShader = NULL;
		mCodeBuffer = NULL;
	}
}