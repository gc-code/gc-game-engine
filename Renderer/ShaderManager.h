#ifndef __SHADERMANAGER_H__
#define __SHADERMANAGER_H__

#define SHADER_DIR "Shaders/"

struct ID3D11ShaderReflectionConstantBuffer;

namespace Renderer
{
	class VertexShader;
	class PixelShader;

	class ShaderManager
	{
	public:

		ShaderManager();

		virtual ~ShaderManager();

		enum EngineShader
		{
			SHADER_TEXTURE,
			SHADER_LEVEL_EDITOR,
		};

		struct EngineShaderDesc
		{
			EngineShader ShaderId;
			const char* ShaderFilePath;
		};

		bool InitShaders();

		VertexShader* GetVertexShader(EngineShader shaderId);
		PixelShader* GetPixelShader(EngineShader shaderId);

		void BindShaderSet(EngineShader shaderId);

		void Free();

	private:
		
		bool ProcessShaderInputs(unsigned int shaderId);
		void ProcessCBInputs(ID3D11ShaderReflectionConstantBuffer* reflection,
			VertexShader* vs);

		// Default shaders loaded every engine session
		static EngineShaderDesc mEngineVertexShaders[];
		static EngineShaderDesc mEnginePixelShaders[];

		std::vector<VertexShader*> mVertexShaderList;
		std::vector<PixelShader*> mPixelShaderList;
	};
}

#endif