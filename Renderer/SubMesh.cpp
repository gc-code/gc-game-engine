#include "Renderer.h"

namespace Renderer
{
	SubMesh::SubMesh()
		: mIndexBuffer(0)
	{}

	SubMesh::~SubMesh()
	{
		mIndices.clear();

		if (mIndexBuffer) delete mIndexBuffer;
	}

	bool SubMesh::CacheIndices()
	{
		RenderSystem* renderSys = RenderManager::GetRenderSystem();
		mIndexBuffer = renderSys->CreateGPUBuffer(&mIndices[0],
			mIndices.size() * sizeof(unsigned short), GPUBuffer::ACCESS_STATIC, GPUBuffer::INDEX_BUFFER);
		if (!mIndexBuffer)
			return false;

		return true;
	}

	void SubMesh::PairWithMaterial(std::vector<Material*> matList)
	{
		for (unsigned int i = 0; i < matList.size(); i++)
		{
			if (matList[i]->GetMaterialName() == mMaterialName)
				mLinkedMaterial = matList[i];
		}
	}

	void SubMesh::Render()
	{
		RenderSystem* renderSys = RenderManager::GetRenderSystem();
		renderSys->BindIndexBuffer(mIndexBuffer);
		mLinkedMaterial->BindTextures();
			
		renderSys->DrawGeomIndexed(mIndices.size());
	}
}