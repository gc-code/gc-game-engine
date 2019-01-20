#include "Renderer.h"

namespace Renderer
{
	Model::Model()
	{
		XMMATRIX world = XMLoadFloat4x4(&mPerModelAttributes.World);
		world = XMMatrixIdentity();
		world = XMMatrixTranspose(world);
		world = XMMatrixScaling(0.2f, 0.3f, 0.3f);

		XMStoreFloat4x4(&mPerModelAttributes.World, world);
	}

	Model::~Model()
	{
		Free();
	}

	bool Model::CreateResources(RenderScene* renderScene)
	{
		mPerModelBuffer = RenderManager::GetRenderSystem()->CreateGPUBuffer(
			&mPerModelAttributes, sizeof(cbPerModel), GPUBuffer::ACCESS_DYNAMIC,
			GPUBuffer::CONSTANT_BUFFER);
		if (!mPerModelBuffer)
			return false;

		for (unsigned int i = 0; i < mMeshGeometryList.size(); i++)
		{
			if (!mMeshGeometryList[i]->CacheMeshVertices())
				return false;
		}
		for (unsigned int i = 0; i < mMaterialList.size(); i++)
		{
			if (!mMaterialList[i]->CreateTextures())
				return false;
		}

		return true;
	}

	void Model::RotateWorldX(float angle)
	{
		XMMATRIX world = XMLoadFloat4x4(&mPerModelAttributes.World);
		world = world * XMMatrixRotationX(angle);
		XMStoreFloat4x4(&mPerModelAttributes.World, world);
	}

	void Model::RotateWorldY(float angle)
	{
		XMMATRIX world = XMLoadFloat4x4(&mPerModelAttributes.World);
		world = world * XMMatrixRotationY(angle);
		XMStoreFloat4x4(&mPerModelAttributes.World, world);
	}

	void Model::RenderModel()
	{
		RenderManager::GetRenderSystem()->UpdateBufferMapFromPrevData(mPerModelBuffer);
		RenderManager::GetRenderSystem()->BindConstantBufferVS(mPerModelBuffer, 1);

		for (unsigned int i = 0; i < mMeshGeometryList.size(); i++)
		{
			mMeshGeometryList[i]->Render();
		}
	}

	void Model::Free()
	{
		delete mPerModelBuffer;

		for (unsigned int i = 0; i < mMeshGeometryList.size(); i++)
		{
			delete mMeshGeometryList[i];
		}
		for (unsigned int i = 0; i < mMaterialList.size(); i++)
		{
			delete mMaterialList[i];
		}
	}
}