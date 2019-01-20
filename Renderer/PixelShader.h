#ifndef __PIXELSHADER_H__
#define __PIXELSHADER_H__

namespace Renderer
{
	class PixelShader
	{
	public:

		PixelShader();

		virtual ~PixelShader();

		void SetD3DPixelShader(ID3D11PixelShader* pixelShader) { mD3DPixelShader = pixelShader; }
		ID3D11PixelShader* GetD3DPixelShader() { return mD3DPixelShader; }

		void Free();

	private:

		ID3D11PixelShader* mD3DPixelShader;
	};
}

#endif