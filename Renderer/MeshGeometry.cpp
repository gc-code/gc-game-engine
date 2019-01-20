#include "Renderer.h"

namespace Renderer
{
	MeshGeometry::MeshGeometry()
		: mHasActiveCache(false)
		, mVertexCache(0)
	{}

	MeshGeometry::~MeshGeometry()
	{
		DeleteVertexCache();

		for (unsigned int i = 0; i < mSubMeshes.size(); i++)
		{
			delete mSubMeshes[i];
		}
	}

	void MeshGeometry::CalculateTangents()
	{
		for (unsigned int i = 0; i < mSubMeshes.size(); i++)
		{
			std::vector<unsigned short>& indices = mSubMeshes[i]->GetIndices();
			XMFLOAT3 *tan1 = new XMFLOAT3[mVertices.size() * 2];
			XMFLOAT3 *tan2 = new XMFLOAT3[mVertices.size() * 2];
			for (unsigned int j = 0; j < indices.size() / 3; j++)
			{
				XMFLOAT3 sdir;
				XMFLOAT3 tdir;

				unsigned short index1 = indices[j * 3 + 0];
				unsigned short index2 = indices[j * 3 + 1];
				unsigned short index3 = indices[j * 3 + 2];

				const XMFLOAT3 vert1 = mVertices[index1].Position;
				const XMFLOAT3 vert2 = mVertices[index2].Position;
				const XMFLOAT3 vert3 = mVertices[index3].Position;

				const XMFLOAT2 tcoord1 = mVertices[index1].UVCoordinate;
				const XMFLOAT2 tcoord2 = mVertices[index2].UVCoordinate;
				const XMFLOAT2 tcoord3 = mVertices[index3].UVCoordinate;

				float x1 = vert2.x - vert1.x;
				float x2 = vert3.x - vert1.x;
				float y1 = vert2.y - vert1.y;
				float y2 = vert3.y - vert1.y;
				float z1 = vert2.z - vert1.z;
				float z2 = vert3.z - vert1.z;

				float s1 = tcoord2.x - tcoord1.x;
				float s2 = tcoord3.x - tcoord1.x;
				float t1 = tcoord2.y - tcoord1.y;
				float t2 = tcoord3.y - tcoord1.y;

				float r = 1.0f / (s1 * t2 - s2 * t1);
				sdir.x = (t2 * x1 - t1 * x2) * r;
				sdir.y = (t2 * y1 - t1 * y2) * r;
				sdir.z = (t2 * z1 - t1 * z2) * r;

				tdir.x = (s1 * x2 - s2 * x1) * r;
				tdir.y = (s1 * y2 - s2 * y1) * r;
				tdir.z = (s1 * z2 - s2 * z1) * r;

				tan1[index1] = sdir;
				tan1[index2] = sdir;
				tan1[index3] = sdir;

				tan2[index1] = tdir;
				tan2[index2] = tdir;
				tan2[index3] = tdir;
			}

			for (unsigned int j = 0; j < mVertices.size(); j++)
			{
				XMFLOAT3 normal = mVertices[j].Normal;

				/* We can use optimized XMVECTOR objects for these calculations */
				XMVECTOR n = XMLoadFloat3(&normal);
				XMVECTOR t = XMLoadFloat3(&tan1[j]);
				XMVECTOR t2 = XMVectorSet(tan2[j].x, tan2[j].y, tan2[j].z, 0.0f);
				XMVECTOR tangent = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				XMVECTOR tangentw = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				float result = 0.0f;

				tangent = (t - n * XMVector3Dot(n, t));
				tangent = XMVector3Normalize(tangent);

				tangentw = (XMVector3Dot(XMVector3Cross(n, t), t2));
				result = XMVectorGetX(tangentw);
				result = (result < 0.0f) ? -1.0f : 1.0f;

				mVertices[j].Tangent.x = XMVectorGetX(tangent);
				mVertices[j].Tangent.y = XMVectorGetY(tangent);
				mVertices[j].Tangent.z = XMVectorGetZ(tangent);
				mVertices[j].Tangent.w = result;
			}

			delete[] tan1;
			delete[] tan2;
		}
		int hello = 0;
	}

	void MeshGeometry::CalculateNormals()
	{
		for (unsigned int i = 0; i < mSubMeshes.size(); i++)
		{
			SubMesh* subMesh = mSubMeshes[i];
			std::vector<unsigned short>& indexList = subMesh->GetIndices();

			for (unsigned int j = 0; j < indexList.size() / 3; j++)
			{
				unsigned short index0 = indexList[j * 3 + 0];
				unsigned short index1 = indexList[j * 3 + 1];
				unsigned short index2 = indexList[j * 3 + 2];

				XMVECTOR point0 = XMLoadFloat3(&mVertices[index0].Position);
				XMVECTOR point1 = XMLoadFloat3(&mVertices[index1].Position);
				XMVECTOR point2 = XMLoadFloat3(&mVertices[index2].Position);

				XMVECTOR faceNormal = XMVector3Normalize(XMVector3Cross(point1 - point0, point2 - point0));

				/* Calculate vertex normal */
				XMVECTOR normal0 = XMLoadFloat3(&mVertices[indexList[index0]].Normal);
				XMVECTOR normal1 = XMLoadFloat3(&mVertices[indexList[index1]].Normal);
				XMVECTOR normal2 = XMLoadFloat3(&mVertices[indexList[index2]].Normal);

				normal0 = normal0 + faceNormal;
				normal1 = normal1 + faceNormal;
				normal2 = normal2 + faceNormal;

				XMStoreFloat3(&mVertices[index0].Normal, normal0);
				XMStoreFloat3(&mVertices[index1].Normal, normal1);
				XMStoreFloat3(&mVertices[index2].Normal, normal2);
			}
		}

		for (unsigned int i = 0; i < mVertices.size(); i++)
		{
			XMVECTOR normal = XMLoadFloat3(&mVertices[i].Normal);
			normal = XMVector3Normalize(normal);
			XMStoreFloat3(&mVertices[i].Normal, normal);
		}
	}

	bool MeshGeometry::CacheMeshVertices()
	{
		RenderSystem* renderSys = RenderManager::GetRenderSystem();
		mVertexCache = renderSys->CreateGPUBuffer(&mVertices[0], mVertices.size() * sizeof(RenderVertex),
			GPUBuffer::ACCESS_STATIC, GPUBuffer::VERTEX_BUFFER);
		if (!mVertexCache)
			return false;

		for (unsigned int i = 0; i < mSubMeshes.size(); i++)
		{
			if (!mSubMeshes[i]->CacheIndices())
				return false;
		}

		mHasActiveCache = true;

		return true;
	}

	void MeshGeometry::Render()
	{
		RenderSystem* renderSys = RenderManager::GetRenderSystem();
		renderSys->BindVertexBuffer(mVertexCache, sizeof(RenderVertex));
		renderSys->SetPrimitiveTopologyTriangles();

		for (unsigned int i = 0; i < mSubMeshes.size(); i++)
		{
			mSubMeshes[i]->Render();
		}

		if (mSubMeshes.size() == 0)
			renderSys->DrawGeom(mVertices.size());
	}

	void MeshGeometry::DeleteVertexCache()
	{
		if (mVertexCache) delete mVertexCache;
		mHasActiveCache = false;
	}
}