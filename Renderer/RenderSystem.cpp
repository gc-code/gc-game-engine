#include "BinaryBuffer.h"
#include "GUIHandler.h"
#include "Renderer.h"

#include "RC/resource.h"

#define RESOURCE_DIR "Resources/"

using namespace Utils;
using namespace System;

typedef BOOL(WINAPI *LPFN_GLPI)(
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION,
	PDWORD);

namespace Renderer
{
	RenderSystem::RenderSystem()
		: mDriverType(D3D_DRIVER_TYPE_NULL)
		, mFeatureLevel(D3D_FEATURE_LEVEL_11_0)
		, mD3DDevice(NULL)
		, mD3DContext(NULL)
		, mSwapChain(NULL)
		, mRenderTargetView(NULL)
		, mDepthStencil(NULL)
		, mDepthStencilView(NULL)
		, mDefaultSampler(NULL)
	{
		mNativeKeyboard = new System::Keyboard();
	}

	RenderSystem::~RenderSystem()
	{
		FreeD3DResources();
	}

	bool RenderSystem::InitWindow(const char* title, unsigned int width, unsigned int height)
	{
		mWindowWidth = width;
		mWindowHeight = height;

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = mHInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
#ifdef IDR_SYSMENU
		wcex.lpszMenuName = MAKEINTRESOURCE(IDR_SYSMENU);
#else
		wcex.lpszMenuName = NULL;
#endif
		wcex.lpszClassName = "PrimaryWindow";
		wcex.hIconSm = NULL;
		if (!RegisterClassEx(&wcex))
			return false;

		RECT rc = { 0, 0, width, height };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		mHWnd = CreateWindow("PrimaryWindow", title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, mHInstance,
			NULL);

		if (!mHWnd)
			return false;

		SetFocus(mHWnd);

		return true;
	}

	void RenderSystem::GetD3DDriverString(char* driverType)
	{
		if (mDriverType == D3D_DRIVER_TYPE_HARDWARE)
			strcpy(driverType, "D3D_DRIVER_TYPE_HARDWARE");
		else if (mDriverType == D3D_DRIVER_TYPE_WARP)
			strcpy(driverType, "D3D_DRIVER_TYPE_WARP");
		else if (mDriverType == D3D_DRIVER_TYPE_REFERENCE)
			strcpy(driverType, "D3D_DRIVER_TYPE_REFERENCE");
		else if (mDriverType == D3D_DRIVER_TYPE_SOFTWARE)
			strcpy(driverType, "D3D_DRIVER_TYPE_SOFTWARE");
	}

	void RenderSystem::GetD3DFeatureLevelString(char* featureLevel)
	{
		if (mFeatureLevel == D3D_FEATURE_LEVEL_10_0)
			strcpy(featureLevel, "D3D_FEATURE_LEVEL_10_0");
		else if (mFeatureLevel == D3D_FEATURE_LEVEL_10_1)
			strcpy(featureLevel, "D3D_FEATURE_LEVEL_10_1");
		else if (mFeatureLevel == D3D_FEATURE_LEVEL_11_0)
			strcpy(featureLevel, "D3D_FEATURE_LEVEL_11_0");
		else if (mFeatureLevel == D3D_FEATURE_LEVEL_11_1)
			strcpy(featureLevel, "D3D_FEATURE_LEVEL_11_1");
	}

	unsigned int RenderSystem::GetNumPhysicalProcessors()
	{
		unsigned int coreCount = 0;

		LPFN_GLPI Glpi;
		Glpi = (LPFN_GLPI)GetProcAddress(
			GetModuleHandle(TEXT("kernel32")),
			"GetLogicalProcessorInformation");
		if (NULL == Glpi)
			return coreCount;

		BOOL done = FALSE;
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
		DWORD returnLength = 0;

		while (!done)
		{
			DWORD rc = Glpi(buffer, &returnLength);

			if (FALSE == rc)
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					if (buffer)
						free(buffer);

					buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);

					if (NULL == buffer)
						return coreCount;
				}
				else
				{
					return coreCount;
				}
			}
			else done = TRUE;
		}

		DWORD byteOffset = 0;
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = buffer;
		while (byteOffset < returnLength)
		{
			if (ptr->Relationship == RelationProcessorCore)
			{
				if (ptr->ProcessorCore.Flags)
				{
					coreCount += 1;
				}
				else
				{
					DWORD LSHIFT = sizeof(ULONG_PTR)* 8 - 1;
					DWORD bitSetCount = 0;
					DWORD bitTest = 1 << LSHIFT;
					DWORD i;

					for (i = 0; i <= LSHIFT; ++i)
					{
						bitSetCount += ((ptr->ProcessorMask & bitTest) ? 1 : 0);
						bitTest /= 2;
					}

					coreCount += bitSetCount;
				}
			}
			byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
			ptr++;
		}

		free(buffer);

		return coreCount;
	}

	bool RenderSystem::InitD3DResources()
	{
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(mHWnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = mHWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			mDriverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain(NULL, mDriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &mSwapChain, &mD3DDevice, &mFeatureLevel, &mD3DContext);

			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
		{
			ReportError("Error: Direct3D device creation failed");
			return false;
		}

		ID3D11Texture2D* pBackBuffer = NULL;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
		{
			ReportError("Error: Couldn't get the swap chain back buffer");
			return false;
		}

		hr = mD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &mRenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
		{
			ReportError("Error: Can't create the render target view");
			return false;
		}

		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = mD3DDevice->CreateTexture2D(&descDepth, NULL, &mDepthStencil);
		if (FAILED(hr))
			return false;

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = mD3DDevice->CreateDepthStencilView(mDepthStencil, &descDSV, &mDepthStencilView);
		if (FAILED(hr))
			return false;

		mD3DContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		mD3DContext->RSSetViewports(1, &vp);

		if (!RenderManager::GetShaderManager()->InitShaders())
		{
			ReportError("Error: Failed to load shaders");
			return false;
		}

		D3D11_RASTERIZER_DESC rastDesc;
		ZeroMemory(&rastDesc, sizeof(rastDesc));
		rastDesc.CullMode = D3D11_CULL_FRONT;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.FillMode = D3D11_FILL_SOLID;

		hr = mD3DDevice->CreateRasterizerState(&rastDesc, &mRasterizerState);
		if (FAILED(hr))
			return false;

		mD3DContext->RSSetState(mRasterizerState);

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = mD3DDevice->CreateSamplerState(&sampDesc, &mDefaultSampler);
		if (FAILED(hr))
			return false;

		mD3DContext->PSSetSamplers(0, 1, &mDefaultSampler);

		return true;
	}

	void RenderSystem::ClearBuffers(float colour[4])
	{
		mD3DContext->ClearRenderTargetView(mRenderTargetView, colour);
		mD3DContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void RenderSystem::SwapBuffers()
	{
		mSwapChain->Present(0, 0);
	}

	void RenderSystem::DisplayWindow(int nCmdShow)
	{
		ShowWindow(mHWnd, nCmdShow);
	}

	GPUBuffer* RenderSystem::CreateGPUBuffer(void* data, unsigned int size,
		GPUBuffer::BufferAccess access, GPUBuffer::BufferType type)
	{
		GPUBuffer* buffer = NULL;

		HRESULT hr = S_OK;
		ID3D11Buffer* d3dBuffer = NULL;
		UINT bindFlags = 0;
		UINT cpuAccess = 0;
		D3D11_USAGE usageFlag = D3D11_USAGE_DEFAULT;

		switch (type)
		{
			case GPUBuffer::VERTEX_BUFFER:
				bindFlags = D3D11_BIND_VERTEX_BUFFER;
				break;

			case GPUBuffer::INDEX_BUFFER:
				bindFlags = D3D11_BIND_INDEX_BUFFER;
				break;

			case GPUBuffer::CONSTANT_BUFFER:
				bindFlags = D3D11_BIND_CONSTANT_BUFFER;
				break;

			default:
				return 0;
		}

		switch (access)
		{
		case GPUBuffer::ACCESS_STATIC:
			usageFlag = D3D11_USAGE_DEFAULT;
			cpuAccess = 0;
			break;

		case GPUBuffer::ACCESS_DYNAMIC:
			usageFlag = D3D11_USAGE_DYNAMIC;
			cpuAccess = D3D11_CPU_ACCESS_WRITE;
			break;

		default:
			return 0;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = usageFlag;
		bd.ByteWidth = size;
		bd.BindFlags = bindFlags;
		bd.CPUAccessFlags = cpuAccess;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = data;
		
		hr = mD3DDevice->CreateBuffer(&bd, &InitData, &d3dBuffer);
		if (FAILED(hr))
			return 0;

		buffer = new GPUBuffer();
		buffer->SetBufferAccess(access);
		buffer->SetBufferType(type);
		buffer->SetD3DBuffer(d3dBuffer);
		buffer->SetDataPointer(data);
		buffer->SetBufferSize(size);

		return buffer;
	}

	void RenderSystem::UpdateBufferMapFromPrevData(GPUBuffer* buffer)
	{
		ID3D11Buffer* d3dBuffer = buffer->GetD3DBuffer();

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		mD3DContext->Map(d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		memcpy(mappedResource.pData, buffer->GetPrevDataPointer(), buffer->GetDataSize());

		mD3DContext->Unmap(d3dBuffer, 0);
	}

	Texture2D* RenderSystem::CreateTexture2D(const char* filePath)
	{
		Texture2D* texture = new Texture2D();
		Image* image = NULL;
		HRESULT hr = S_OK;
		ID3D11ShaderResourceView* srv = NULL;

		std::string name = filePath;

		if (name.find(".tga"))
			image = new TGAImage();
		else
			return false;

		if (!image->Load(((std::string)RESOURCE_DIR + name).c_str()))
			return false;

		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.Width = image->GetWidth();
		TexDesc.Height = image->GetHeight();
		TexDesc.MipLevels = 0;
		TexDesc.ArraySize = 1;
		TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		TexDesc.SampleDesc.Count = 1;
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		ID3D11Texture2D* tex = nullptr;
		hr = mD3DDevice->CreateTexture2D(&TexDesc, nullptr, &tex);
		if (FAILED(hr))
			return false;

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		SRVDesc.Format = TexDesc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = -1;

		hr = mD3DDevice->CreateShaderResourceView(tex, &SRVDesc, &srv);
		if (FAILED(hr))
		{
			return false;
		}

		mD3DContext->UpdateSubresource(tex, 0, nullptr, image->GetPixelData(), image->GetWidth() * image->GetNumComponents(), image->GetPixelDataSize());
		mD3DContext->GenerateMips(srv);
		tex->Release();

		texture->SetShaderResourceView(srv);

		return texture;
	}

	VertexShader* RenderSystem::CreateVertexShader(const char* filePath)
	{
		HRESULT hr = S_OK;
		ID3D11VertexShader* d3dVertexShader = NULL;
		VertexShader* vertexShader = NULL;
		BinaryBuffer* binaryBuffer = new BinaryBuffer(filePath);

		hr = mD3DDevice->CreateVertexShader(binaryBuffer->GetBuffer(), binaryBuffer->GetBufferSize(),
			NULL, &d3dVertexShader);
		if (FAILED(hr))
			return 0;

		vertexShader = new VertexShader();
		vertexShader->SetD3DVertexShader(d3dVertexShader);
		vertexShader->SetCodeBuffer(binaryBuffer);

		return vertexShader;
	}

	PixelShader* RenderSystem::CreatePixelShader(const char* filePath)
	{
		PixelShader* pixelShader = NULL;
		HRESULT hr = S_OK;
		ID3D11PixelShader* d3dPixelShader = NULL;
		BinaryBuffer* binaryBuffer = new BinaryBuffer(filePath);

		hr = mD3DDevice->CreatePixelShader(binaryBuffer->GetBuffer(), binaryBuffer->GetBufferSize(),
			NULL, &d3dPixelShader);
		if (FAILED(hr))
			return 0;

		pixelShader = new PixelShader();
		pixelShader->SetD3DPixelShader(d3dPixelShader);

		return pixelShader;
	}

	void RenderSystem::BindConstantBufferVS(GPUBuffer* buffer, unsigned int port)
	{
		ID3D11Buffer* d3dBuffer = buffer->GetD3DBuffer();
		mD3DContext->VSSetConstantBuffers(port, 1, &d3dBuffer);
	}

	void RenderSystem::BindConstantBufferPS(GPUBuffer* buffer, unsigned int port)
	{
		ID3D11Buffer* d3dBuffer = buffer->GetD3DBuffer();
		mD3DContext->PSSetConstantBuffers(port, 1, &d3dBuffer);
	}

	void RenderSystem::BindVertexBuffer(GPUBuffer* vertexBuffer, unsigned int stride)
	{
		ID3D11Buffer* d3dVertBuf = vertexBuffer->GetD3DBuffer();
		unsigned int offset = 0;
		mD3DContext->IASetVertexBuffers(0, 1, &d3dVertBuf, &stride, &offset);
	}

	void RenderSystem::BindIndexBuffer(GPUBuffer* indexBuffer)
	{
		ID3D11Buffer* d3dIndexBuf = indexBuffer->GetD3DBuffer();
		mD3DContext->IASetIndexBuffer(d3dIndexBuf, DXGI_FORMAT_R16_UINT, 0);
	}

	void RenderSystem::BindTexture2D(Texture2D* texture, int slot)
	{
		ID3D11ShaderResourceView* srv = texture->GetShaderResourceView();
		mD3DContext->PSSetShaderResources(slot, 1, &srv);
	}

	void RenderSystem::BindVertexShader(VertexShader* vertexShader)
	{
		ID3D11VertexShader* vs = vertexShader->GetD3DVertexShader();
		mD3DContext->VSSetShader(vs, NULL, 0);
		mD3DContext->IASetInputLayout(vertexShader->GetInputLayout());
	}

	void RenderSystem::BindPixelShader(PixelShader* pixelShader)
	{
		ID3D11PixelShader* ps = pixelShader->GetD3DPixelShader();
		mD3DContext->PSSetShader(ps, NULL, 0);
	}

	void RenderSystem::DrawGeomIndexed(unsigned int numIndices)
	{
		mD3DContext->DrawIndexed(numIndices, 0, 0);
	}

	void RenderSystem::DrawGeom(unsigned int vertexCount)
	{
		mD3DContext->Draw(vertexCount, 0);
	}

	void RenderSystem::FreeD3DResources()
	{
		if (mD3DContext) mD3DContext->ClearState();

		if (mDefaultSampler) mDefaultSampler->Release();
		if (mRasterizerState) mRasterizerState->Release();
		if (mDepthStencil) mDepthStencil->Release();
		if (mDepthStencilView) mDepthStencilView->Release();
		if (mRenderTargetView) mRenderTargetView->Release();
		if (mSwapChain) mSwapChain->Release();
		if (mD3DContext) mD3DContext->Release();
		if (mD3DDevice) mD3DDevice->Release();
	}

	void RenderSystem::ReportError(const char* errorText)
	{
		MessageBox(NULL, errorText, "Critical Error", MB_OK);
	}

	void RenderSystem::SetPrimitiveTopologyTriangles()
	{
		mD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void RenderSystem::SetPrimitiveTopologyTristrips()
	{
		mD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	LRESULT CALLBACK RenderSystem::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rcClient;
		POINT ptClientUL;
		POINT ptClientLR;
		static POINTS ptsBegin;
		static POINTS ptsEnd;
		static POINTS ptsPrevEnd;
		OPENFILENAMEA fileStruct;

		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_CREATE:
#ifdef IDR_SYSMENU
			HMENU hMenu, hSubMenu;

			hMenu = CreateMenu();
			hSubMenu = CreatePopupMenu();

			AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, "&Open");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

			SetMenu(hWnd, hMenu);
#endif
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case ID_FILE_OPEN:
				char fileName[1000];
				fileName[0] = '\0';

				ZeroMemory(&fileStruct, sizeof(OPENFILENAMEA));
				fileStruct.lStructSize = sizeof(fileStruct);
				fileStruct.lpstrFilter = "Model Files (*.mdl)\0*.mdl\0Map Files (*.hmp)\0*.hmp\0TARGA Height Map (*.tga)\0*.tga\0";
				fileStruct.lpstrFile = fileName;
				fileStruct.nMaxFile = 1000;
				fileStruct.lpstrTitle = "Open";
				fileStruct.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
				
				if (GetOpenFileNameA(&fileStruct))
					GUIHandler::ProcessFileString(fileStruct.lpstrFile);
				break;
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE:
				Keyboard::RegisterKeyEvent(Keyboard::KEY_ESC);
				break;
			case VK_LEFT:
				Keyboard::RegisterKeyEvent(Keyboard::KEY_LEFT);
				break;
			case VK_RIGHT:
				Keyboard::RegisterKeyEvent(Keyboard::KEY_RIGHT);
				break;
			case VK_UP:
				Keyboard::RegisterKeyEvent(Keyboard::KEY_UP);
				break;
			case VK_DOWN:
				Keyboard::RegisterKeyEvent(Keyboard::KEY_DOWN);
				break;
			}
			break;

		case WM_KEYUP:
			Keyboard::ClearState();
			break;

		case WM_LBUTTONDOWN:
			Mouse::RegisterButtonPress(Mouse::MOUSE_LEFT_CLICK);

			// Retrieve the screen coordinates of the client area
			// and convert them into client coordinates

			GetClientRect(hWnd, &rcClient);
			ptClientUL.x = rcClient.left;
			ptClientUL.y = rcClient.top;

			// Add one to the right and bottom sides, because the 
			// coordinates retrieved by GetClientRect do not 
			// include the far left and lowermost pixels.

			ptClientLR.x = rcClient.right + 1;
			ptClientLR.y = rcClient.bottom + 1;
			ClientToScreen(hWnd, &ptClientUL);
			ClientToScreen(hWnd, &ptClientLR);

			SetRect(&rcClient, ptClientUL.x, ptClientUL.y,
				ptClientLR.x, ptClientLR.y);

			ptsBegin = MAKEPOINTS(lParam);

			// Update the mouse object
			Mouse::SetScreenCoordX(ptsBegin.x);
			Mouse::SetScreenCoordY(ptsBegin.y);
			break;

		case WM_RBUTTONDOWN:
			Mouse::RegisterButtonPress(Mouse::MOUSE_RIGHT_CLICK);
			break;

		case WM_MBUTTONDOWN:
			Mouse::RegisterButtonPress(Mouse::MOUSE_MIDDLE_BUTTON);
			break;

		case WM_LBUTTONUP:
			Mouse::ClearState();
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
}