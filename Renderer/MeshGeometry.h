#ifndef __MESHGEOMETRY_H__
#define __MESHGEOMETRY_H__

struct ID3D11Buffer;

namespace Mechanic
{
	class Armature;
}

namespace Renderer
{
	class GPUBuffer;
	class RenderScene;

	struct RenderVertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 UVCoordinate;
		XMFLOAT3 Normal;
		XMFLOAT4 Tangent;
	};

	class MeshGeometry
	{
	public:

		MeshGeometry();

		virtual ~MeshGeometry();

		bool CacheMeshVertices();	// Create a GPU cache of the vertex data and indices

		void CalculateAnimPose(Mechanic::Armature* armature);

		std::vector<RenderVertex>& GetVertexData() { return mVertices; }
		std::vector<SubMesh*>& GetSubMeshes() { return mSubMeshes; }

		void SetHasPositions(bool hasPos) { mHasPositions = hasPos; }
		void SetHasUVCoords(bool hasUV) { mHasUVCoords = hasUV; }
		void SetHasNormals(bool hasNorms) { mHasNormals = hasNorms; }

		bool HasPositions() { return mHasPositions; }
		bool HasUVCoords() { return mHasUVCoords; }
		bool HasNormals() { return mHasNormals; }

		void CalculateTangents();
		void CalculateNormals();

		void Render();

		void DeleteVertexCache();

	private:

		std::vector<RenderVertex> mVertices;
		std::vector<SubMesh*> mSubMeshes;

		GPUBuffer* mVertexCache;

		bool mHasActiveCache;
		bool mHasComputedTangents;
		
		bool mHasPositions;
		bool mHasUVCoords;
		bool mHasNormals;
	};
}

#endif