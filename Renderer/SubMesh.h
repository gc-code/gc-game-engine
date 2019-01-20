#ifndef __SUBMESH_H__
#define __SUBMESH_H__

namespace Renderer
{
	class GPUBuffer;
	class Material;

	class SubMesh
	{
	public:

		SubMesh();

		virtual ~SubMesh();

		enum TriangleFormat
		{
			TRI_FORMAT_TRIANGLES,
			TRI_FORMAT_TRI_STRIPS,
			TRI_FORMAT_TRI_FANS,
		};

		bool CacheIndices();

		void SetTriangleFormat(TriangleFormat triFormat) { mTriFormat = triFormat; }

		void SetMaterialName(std::string matName) { mMaterialName = matName; }

		void PairWithMaterial(std::vector<Material*> matList);

		void Render();

		std::vector<unsigned short>& GetIndices() { return mIndices; }

	private:

		std::string mMaterialName;
		Material* mLinkedMaterial;

		std::vector<unsigned short> mIndices;
		TriangleFormat mTriFormat;

		GPUBuffer* mIndexBuffer;
	};
}

#endif