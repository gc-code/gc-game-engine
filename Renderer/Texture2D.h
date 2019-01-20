#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

namespace Renderer
{
	class Texture2D
	{
	public:

		Texture2D();

		virtual ~Texture2D();

		ID3D11ShaderResourceView* GetShaderResourceView() { return mD3DShaderResourceView; }
		void SetShaderResourceView(ID3D11ShaderResourceView* srv) { mD3DShaderResourceView = srv; }

		void Free();

	private:

		ID3D11ShaderResourceView* mD3DShaderResourceView;
	};
}

#endif