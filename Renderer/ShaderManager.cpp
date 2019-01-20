#include <d3dcompiler.inl>
#include "BinaryBuffer.h"
#include "Renderer.h"

using namespace Utils;

namespace Renderer
{
	ShaderManager::EngineShaderDesc ShaderManager::mEngineVertexShaders[] =
	{
		{ ShaderManager::SHADER_TEXTURE, "TextureVS.cso" },
		{ ShaderManager::SHADER_LEVEL_EDITOR, "LevelEditorVS.cso" },
	};

	ShaderManager::EngineShaderDesc ShaderManager::mEnginePixelShaders[] =
	{
		{ ShaderManager::SHADER_TEXTURE, "TexturePS.cso" },
		{ ShaderManager::SHADER_LEVEL_EDITOR, "LevelEditorPS.cso" },
	};

	ShaderManager::ShaderManager()
	{
	}

	ShaderManager::~ShaderManager()
	{
		Free();
	}

	bool ShaderManager::InitShaders()
	{
		ID3D11Device* device = RenderManager::GetRenderSystem()->GetD3DDevice();

		int numShaders = sizeof(mEngineVertexShaders) / sizeof(mEngineVertexShaders[0]);
		RenderSystem* renderSystem = RenderManager::GetRenderSystem();
		mVertexShaderList.reserve(numShaders);
		mPixelShaderList.reserve(numShaders);

		for (int i = 0; i < numShaders; i++)
		{
			VertexShader* vertexShader = NULL;
			PixelShader* pixelShader = NULL;

			std::string vsPath = (std::string)SHADER_DIR + mEngineVertexShaders[i].ShaderFilePath;
			std::string psPath = (std::string)SHADER_DIR + mEnginePixelShaders[i].ShaderFilePath;

			vertexShader = renderSystem->CreateVertexShader(vsPath.c_str());
			if (!vertexShader)
				return false;

			pixelShader = renderSystem->CreatePixelShader(psPath.c_str());
			if (!pixelShader)
				return false;

			mVertexShaderList.push_back(vertexShader);
			mPixelShaderList.push_back(pixelShader);

			ProcessShaderInputs(mEngineVertexShaders[i].ShaderId);
		}

		return true;
	}

	VertexShader* ShaderManager::GetVertexShader(EngineShader shaderId)
	{
		return mVertexShaderList[shaderId];
	}

	PixelShader* ShaderManager::GetPixelShader(EngineShader shaderId)
	{
		return mPixelShaderList[shaderId];
	}

	void ShaderManager::BindShaderSet(EngineShader shaderId)
	{
		RenderSystem* renderSystem = RenderManager::GetRenderSystem();
		VertexShader* vertexShader = GetVertexShader(shaderId);
		PixelShader* pixelShader = GetPixelShader(shaderId);

		renderSystem->BindVertexShader(vertexShader);
		renderSystem->BindPixelShader(pixelShader);
	}

	bool ShaderManager::ProcessShaderInputs(unsigned int shaderId)
	{
		ID3D11Device* device = RenderManager::GetRenderSystem()->GetD3DDevice();
		D3D11_SHADER_DESC vsDesc;
		ID3D11ShaderReflection *vertexReflect = 0;

		VertexShader* vs = GetVertexShader((EngineShader)shaderId);
		BinaryBuffer* vsCode = vs->GetCodeBuffer();

		D3D11Reflect(vsCode->GetBuffer(), vsCode->GetBufferSize(), &vertexReflect);
		vertexReflect->GetDesc(&vsDesc);

		if (!vs->HasInputLayout())
		{
			unsigned int numInputs = vsDesc.InputParameters;
			unsigned int stride = 0;
			D3D11_INPUT_ELEMENT_DESC* inputs = new D3D11_INPUT_ELEMENT_DESC[numInputs];
			ID3D11InputLayout* layout = 0;

			for (unsigned int i = 0; i < numInputs; i++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC sigParamDesc;
				vertexReflect->GetInputParameterDesc(i, &sigParamDesc);
				D3D11_INPUT_ELEMENT_DESC input;

				if ((std::string)sigParamDesc.SemanticName == "POSITION")
				{
					input = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, stride, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					stride += 12;
				}
				else if ((std::string)sigParamDesc.SemanticName == "TEXCOORD")
				{
					input = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, stride, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					stride += 8;
				}
				else if ((std::string)sigParamDesc.SemanticName == "NORMAL")
				{
					input = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, stride, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					stride += 12;
				}
				else if ((std::string)sigParamDesc.SemanticName == "TANGENT")
				{
					input = { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, stride, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					stride += 16;
				}
				else
				{
					continue;
				}

				inputs[i] = input;
			}

			HRESULT hr = device->CreateInputLayout(inputs, numInputs, vsCode->GetBuffer(), vsCode->GetBufferSize(), &layout);
			if (FAILED(hr))
				return false;

			vs->SetInputLayout(layout);
		}

		for (unsigned int i = 0; i < vsDesc.ConstantBuffers; i++)
		{
			ID3D11ShaderReflectionConstantBuffer* cbReflect
				= vertexReflect->GetConstantBufferByIndex(i);

			ProcessCBInputs(cbReflect, vs);
		}

		return true;
	}

	void ShaderManager::ProcessCBInputs(ID3D11ShaderReflectionConstantBuffer* cbReflect,
		VertexShader* vs)
	{
		D3D11_SHADER_BUFFER_DESC cbDesc;
		cbReflect->GetDesc(&cbDesc);
		std::string cbName = cbDesc.Name;

		/* I have standardized slots here to reduce complexity */
		if (cbName == "cbPerLevel") vs->SetHasLevelCB(true);	// Slot 1
		if (cbName == "cbPerModel") vs->SetHasModelCB(true);	// Slot 2
		if (cbName == "cbMaterialAttributes") vs->SetHasMaterialCB(true);	// Slot 3
		if (cbName == "cbSkinning") vs->SetHasSkinningCB(true);		// Slot4
	}

	void ShaderManager::Free()
	{
		for (unsigned int i = 0; i < mVertexShaderList.size(); i++)
		{
			delete mVertexShaderList[i];
			delete mPixelShaderList[i];
		}
	}
}