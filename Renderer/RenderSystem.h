#ifndef __RENDERSYSTEM_H__
#define __RENDERSYSTEM_H__

namespace Renderer
{
	enum GUIEvents
	{
		EVENT_NONE,
		EVENT_FILE_STRING,
		EVENT_RESIZE_WINDOW,
		EVENT_FILE_DIALOGUE,
	};

	class RenderSystem
	{
	public:

		RenderSystem();

		virtual ~RenderSystem();

		bool InitWindow(const char* title, unsigned int width, unsigned int height);

		bool InitD3DResources();

		void ClearBuffers(float colour[4]);
		void SwapBuffers();

		void DisplayWindow(int nCmdShow);

		void GetD3DDriverString(char *driverType);
		void GetD3DFeatureLevelString(char *featureLevel);

		unsigned int GetNumPhysicalProcessors();

		/* Resource creation */
		GPUBuffer* CreateGPUBuffer(void* data, unsigned int size,
			GPUBuffer::BufferAccess access, GPUBuffer::BufferType type);

		void UpdateBufferMapFromPrevData(GPUBuffer* buffer);

		Texture2D* CreateTexture2D(const char* filePath);

		VertexShader* CreateVertexShader(const char* filePath);

		PixelShader* CreatePixelShader(const char* filePath);

		/* Resource binding */
		void BindVertexBuffer(GPUBuffer* vertexBuffer, unsigned int stride);

		void BindIndexBuffer(GPUBuffer* indexBuffer);

		void BindTexture2D(Texture2D* texture, int slot);

		void BindVertexShader(VertexShader* vertexShader);
		void BindPixelShader(PixelShader* pixelShader);

		void BindConstantBufferVS(GPUBuffer* buffer, unsigned int port);
		void BindConstantBufferPS(GPUBuffer* buffer, unsigned int port);

		void DrawGeomIndexed(unsigned int numIndices);
		void DrawGeom(unsigned int vertexCount);

		ID3D11Device* GetD3DDevice() { return mD3DDevice; }
		ID3D11DeviceContext* GetD3DContext() { return mD3DContext; }

		static void ReportError(const char* errorText);

		System::Keyboard* GetNativeKeyboard() { return mNativeKeyboard; }

		unsigned int GetWindowHeight() { return mWindowHeight; }
		unsigned int GetWindowWidth() { return mWindowWidth; }

		void SetPrimitiveTopologyTriangles();
		void SetPrimitiveTopologyTristrips();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		void FreeD3DResources();

	private:

		// Windows resources
		HINSTANCE mHInstance;
		HWND mHWnd;

		unsigned int mWindowWidth;
		unsigned int mWindowHeight;

		// D3D resources
		D3D_DRIVER_TYPE mDriverType;
		D3D_FEATURE_LEVEL mFeatureLevel;

		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DContext;
		IDXGISwapChain* mSwapChain;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11Texture2D* mDepthStencil;
		ID3D11DepthStencilView* mDepthStencilView;
		ID3D11RasterizerState* mRasterizerState;
		ID3D11SamplerState* mDefaultSampler;

		System::Keyboard* mNativeKeyboard;
	};
}

#endif