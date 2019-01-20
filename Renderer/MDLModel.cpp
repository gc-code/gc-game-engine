#include "BinaryBuffer.h"
#include "Renderer.h"

#define RESOURCE_DIR "Resources/"

/*
	This is a tool for reading MDL model information.
	The format is designed to be as fast as possible to load.
	This means it contains the minimum possible information while still attempting
	to provide all the data needed so the engine can create some cool effects.
	Ask me for the Blender 3D exporter.
*/

using namespace Renderer;
using namespace Mechanic;
using namespace Utils;

namespace Renderer
{
	MDLModel::MDLModel()
	{}

	MDLModel::~MDLModel()
	{}

	bool MDLModel::Load(const char* filePath)
	{
		BinaryBuffer* binaryBuffer = new BinaryBuffer();
		if (!binaryBuffer->ReadFromFile(((std::string)RESOURCE_DIR + filePath).c_str()))
			return false;

		mFileVersion = binaryBuffer->ReadLittleByte();
		if (mFileVersion != MDL_LOADER_VERSION)
			return false;	// The MDL version does not match that of the loader

		mNumMaterials = binaryBuffer->ReadLittleByte();
		mNumMeshes = binaryBuffer->ReadLittleByte();
		unsigned char hasArmature = binaryBuffer->ReadLittleByte();

		mMaterialList.reserve(mNumMaterials);
		mMeshGeometryList.reserve(mNumMeshes);

		unsigned short matCode;
		XMFLOAT3 tempFloat;
		char tempStr[64];
		for (unsigned int i = 0; i < mNumMaterials; i++)
		{
			Material* material = new Material();
			bool endReached = false;

			binaryBuffer->ReadString(tempStr, 64);
			material->SetMaterialName(tempStr);

			while (endReached != true)
			{
				matCode = binaryBuffer->ReadLittleWord();
				switch (matCode)
				{
				case MAT_AMBIENT:
					tempFloat.x = binaryBuffer->ReadLittleFloat();
					tempFloat.y = binaryBuffer->ReadLittleFloat();
					tempFloat.z = binaryBuffer->ReadLittleFloat();
					material->SetAmbientColour(tempFloat);
					break;

				case MAT_DIFFUSE:
					tempFloat.x = binaryBuffer->ReadLittleFloat();
					tempFloat.y = binaryBuffer->ReadLittleFloat();
					tempFloat.z = binaryBuffer->ReadLittleFloat();
					material->SetDiffuseColour(tempFloat);
					break;

				case MAT_SPECULAR:
					tempFloat.x = binaryBuffer->ReadLittleFloat();
					tempFloat.y = binaryBuffer->ReadLittleFloat();
					tempFloat.z = binaryBuffer->ReadLittleFloat();
					material->SetSpecularColour(tempFloat);
					break;

				case MAT_DIFFUSE_MAP:
					binaryBuffer->ReadString(tempStr, 64);
					material->SetDiffuseMapPath(tempStr);
					break;

				case MAT_SPECULAR_MAP:
					binaryBuffer->ReadString(tempStr, 64);
					material->SetSpecularMapPath(tempStr);
					break;

				case MAT_BUMP_MAP:
					binaryBuffer->ReadString(tempStr, 64);
					material->SetBumpMapPath(tempStr);
					break;

				case MAT_END:
					endReached = true;
					break;

				default:
					return false;
				}
			}

			mMaterialList.push_back(material);
		}

		for (unsigned int i = 0; i < mNumMeshes; i++)
		{
			MeshGeometry* geometry = new MeshGeometry();
			unsigned short numVertices = binaryBuffer->ReadLittleWord();
			unsigned short numUVCoords = binaryBuffer->ReadLittleWord();
			unsigned char numSubMeshes = binaryBuffer->ReadLittleByte();

			geometry->SetHasPositions(true);
			geometry->SetHasUVCoords(true);
			geometry->SetHasNormals(true);

			if (numVertices == 0)
				geometry->SetHasPositions(false);
			if (numUVCoords == 0)
				geometry->SetHasUVCoords(false);

			std::vector<RenderVertex>& vertices = geometry->GetVertexData();
			std::vector<SubMesh*>& subMeshes = geometry->GetSubMeshes();
			vertices.reserve(numVertices);

			for (unsigned int j = 0; j < numVertices; j++)
			{
				RenderVertex vertex;
				vertex.Position.x = binaryBuffer->ReadLittleFloat();
				vertex.Position.z = binaryBuffer->ReadLittleFloat();
				vertex.Position.y = binaryBuffer->ReadLittleFloat();

				vertex.Normal.x = 0.0f;
				vertex.Normal.y = 0.0f;
				vertex.Normal.z = 0.0f;

				vertices.push_back(vertex);
			}

			for (unsigned int j = 0; j < numUVCoords; j++)
			{
				RenderVertex* vertex = &vertices[j];
				vertex->UVCoordinate.x = binaryBuffer->ReadLittleFloat();
				vertex->UVCoordinate.y = binaryBuffer->ReadLittleFloat();
			}

			for (unsigned int j = 0; j < numSubMeshes; j++)
			{
				SubMesh* subMesh = new SubMesh();
				binaryBuffer->ReadString(tempStr, 64);
				subMesh->SetMaterialName(tempStr);
				subMesh->PairWithMaterial(mMaterialList);
				unsigned short numTris = binaryBuffer->ReadLittleWord();
				std::vector<unsigned short>& indices = subMesh->GetIndices();
				indices.reserve(numTris);

				for (unsigned int k = 0; k < numTris; k++)
				{
					unsigned short index1, index2, index3;
					index1 = binaryBuffer->ReadLittleWord();
					index2 = binaryBuffer->ReadLittleWord();
					index3 = binaryBuffer->ReadLittleWord();

					indices.push_back(index1);
					indices.push_back(index2);
					indices.push_back(index3);
				}

				subMeshes.push_back(subMesh);
			}

			geometry->CalculateNormals();
			geometry->CalculateTangents();

			mMeshGeometryList.push_back(geometry);
		}

		delete binaryBuffer;
		return true;
	}
}