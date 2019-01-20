#ifndef __VERTEXSHADER_H__
#define __VERTEXSHADER_H__

namespace Utils
{
	class BinaryBuffer;
}

namespace Renderer
{
	class VertexShader
	{
	public:

		VertexShader();

		virtual ~VertexShader();

		void SetD3DVertexShader(ID3D11VertexShader* vs) { mD3DVertexShader = vs; }
		ID3D11VertexShader* GetD3DVertexShader() { return mD3DVertexShader; }

		ID3D11InputLayout* GetInputLayout() { return mInputLayout; }

		void SetCodeBuffer(Utils::BinaryBuffer* codeBuf) { mCodeBuffer = codeBuf; }
		Utils::BinaryBuffer* GetCodeBuffer() { return mCodeBuffer; }

		void SetInputLayout(ID3D11InputLayout* input) { mInputLayout = input; }
		bool HasInputLayout();

		void SetHasModelCB(bool hasModelCB) { mHasModelCB = hasModelCB; }
		void SetHasLevelCB(bool hasLevelCB) { mHasLevelCB = hasLevelCB; }
		void SetHasMaterialCB(bool hasMaterialCB) { mHasMaterialCB = hasMaterialCB; }
		void SetHasSkinningCB(bool hasSkinningCB) { mHasSkinningCB = hasSkinningCB; }

		bool HasModelCB() { return mHasModelCB; }
		bool HasLevelCB() { return mHasLevelCB; }
		bool HasMaterialCB() { return mHasMaterialCB; }
		bool HasSkinnningCB() { return mHasSkinningCB; }

		void Free();

	private:

		Utils::BinaryBuffer* mCodeBuffer;
		ID3D11VertexShader* mD3DVertexShader;

		ID3D11InputLayout* mInputLayout;

		bool mHasModelCB;
		bool mHasLevelCB;
		bool mHasMaterialCB;
		bool mHasSkinningCB;
	};
}

#endif