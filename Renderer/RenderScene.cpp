#include "Renderer.h"

namespace Renderer
{
	RenderScene::RenderScene()
		: mWorldMap(0)
	{
		SetProjectionMatrixPerspective();
	}

	RenderScene::~RenderScene()
	{
		for (unsigned int i = 0; i < mModelsList.size(); i++)
		{
			delete mModelsList[i];
		}

		if (mPerLevelBuffer)
			delete mPerLevelBuffer;
	}

	bool RenderScene::CacheData()
	{
		if (mWorldMap)
		{
			if (!mWorldMap->CacheData())
				return false;
		}

		mPerLevelBuffer = RenderManager::GetRenderSystem()->CreateGPUBuffer(
			&mPerLevelAttributes, sizeof(cbPerLevel), GPUBuffer::ACCESS_STATIC,
			GPUBuffer::CONSTANT_BUFFER);
		if (!mPerLevelBuffer)
			return false;

		for (unsigned int i = 0; i < mModelsList.size(); i++)
		{
			if (!mModelsList[i]->CreateResources(this))
				return false;
		}

		return true;
	}

	void RenderScene::SetProjectionMatrixPerspective()
	{
		XMMATRIX projection = XMLoadFloat4x4(&mPerLevelAttributes.Projection);
		unsigned int width = RenderManager::GetRenderSystem()->GetWindowWidth();
		unsigned int height = RenderManager::GetRenderSystem()->GetWindowHeight();

		projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)(width / height), 0.01f, 100.0f);
		projection = XMMatrixTranspose(projection);

		XMStoreFloat4x4(&mPerLevelAttributes.Projection, projection);
	}

	void RenderScene::SetViewMatrixLookAt()
	{
		XMMATRIX view = XMLoadFloat4x4(&mPerLevelAttributes.View);
		XMVECTOR eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
		XMVECTOR at = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		view = XMMatrixLookAtLH(eye, at, up);
		view = XMMatrixTranspose(view);

		XMStoreFloat4x4(&mPerLevelAttributes.View, view);
	}

	void RenderScene::Render()
	{
		RenderSystem* renderSys = RenderManager::GetRenderSystem();
		renderSys->BindConstantBufferVS(mPerLevelBuffer, 0);

		if (mWorldMap) mWorldMap->Render();

		for (unsigned int i = 0; i < mModelsList.size(); i++)
		{
			mModelsList[i]->RenderModel();
		}

		renderSys->SwapBuffers();
	}
}