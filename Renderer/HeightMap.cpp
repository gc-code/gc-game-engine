#include "Renderer.h"
#include "BinaryBuffer.h"

#define MAP_RESOLUTION 5.0f
#define RESOURCE_DIR "Resources/"

namespace Renderer
{
	HeightMap::HeightMap()
		: mRawImageData(0)
		, mMeshData(0)
		, mDefaultMaterial(0)
	{
		XMMATRIX world = XMLoadFloat4x4(&mPerModelAttributes.World);
		world = XMMatrixIdentity();
		world = XMMatrixTranspose(world);
		world = XMMatrixScaling(0.2f, 0.3f, 0.3f);

		XMStoreFloat4x4(&mPerModelAttributes.World, world);
	}

	HeightMap::~HeightMap()
	{
		if (mRawImageData) delete mRawImageData;
		if (mMeshData) delete mMeshData;
		if (mDefaultMaterial) delete mDefaultMaterial;
	}

	bool HeightMap::LoadMapFromImage(const char* dataFile)
	{
		mMeshData = new MeshGeometry();

		std::string fileName = dataFile;
		if (fileName.find(".tga"))
			mRawImageData = new TGAImage();
		else
			return false;

		if (!mRawImageData->Load(((std::string)RESOURCE_DIR + dataFile).c_str()))
			return false;

		unsigned char* imageDataPtr = mRawImageData->GetPixelData();
		int imgWidth = mRawImageData->GetWidth();
		int imgHeight = mRawImageData->GetHeight();
		int numPixels = imgWidth * imgHeight * 3;

		std::vector<RenderVertex>& renderVertices = mMeshData->GetVertexData();
		renderVertices.reserve(numPixels / 4);

		float finalX, finalZ;
		for (int i = 0; i < imgHeight; i += (int) MAP_RESOLUTION)
		{
			for (int j = 0; j < imgWidth; j += (int) MAP_RESOLUTION)
			{
				for (int k = 0; k < 6; k++)
				{
					RenderVertex vertex;

					finalX = (float)i + ((k == 1 || k == 2 || k == 5) ? MAP_RESOLUTION : 0.0f);
					finalZ = (float)j + ((k == 1 || k == 4 || k == 5) ? MAP_RESOLUTION : 0.0f);

					vertex.Position.x = (finalX - (imgWidth / 2)) * 0.3f;

					int arrayIndex = (((int)finalX % imgWidth) + (((int)finalZ % imgHeight) * imgWidth)) * 4;
					float blue = (float)imageDataPtr[arrayIndex];
					float green = (float)imageDataPtr[arrayIndex + 1];
					float red = (float)imageDataPtr[arrayIndex + 2];
					vertex.Position.y = (0.299f * red + 0.587f * green + 0.114f * blue) * 0.1f;

					vertex.Position.z = (finalZ - (imgHeight / 2)) * 0.3f;

					vertex.UVCoordinate.x = finalX / (imgWidth / MAP_RESOLUTION);
					vertex.UVCoordinate.y = finalZ / (imgHeight / MAP_RESOLUTION);

					renderVertices.push_back(vertex);
				}
			}
		}

		return true;
	}

	bool HeightMap::CacheData()
	{
		if (!mMeshData->CacheMeshVertices())
			return false;

		return true;
	}

	bool HeightMap::SaveMapAsHMP(const char* fileName)
	{
		Utils::BinaryBuffer buffer;
		unsigned char numHeightMaps = 0;
		unsigned char numEntities = 0;
		unsigned int fileSize = HMP_BASE_DATA_SIZE;
		unsigned int imageOutBufferSize = mRawImageData->GetWidth() * mRawImageData->GetHeight() * 3;
		unsigned char* imageOutBuffer = 0;
		unsigned char* rawImageBuffer = mRawImageData->GetPixelData();

		imageOutBuffer = new unsigned char[imageOutBufferSize];
		for (unsigned int i = 0; i < mRawImageData->GetPixelDataSize() / 4; i++)
		{
			imageOutBuffer[i * 3 + 0] = rawImageBuffer[i * 4 + 2];
			imageOutBuffer[i * 3 + 1] = rawImageBuffer[i * 4 + 1];
			imageOutBuffer[i * 3 + 2] = rawImageBuffer[i * 4 + 0];
		}
		fileSize += imageOutBufferSize;
		buffer.AllocateFileBuffer(fileSize);

		buffer.WriteLittleByte(HMP_VERSION);
		buffer.WriteLittleByteArray(imageOutBuffer, imageOutBufferSize);

		buffer.WriteToFile(fileName);

		delete[] imageOutBuffer;

		return true;
	}

	void HeightMap::Render()
	{
		mDefaultMaterial->BindTextures();
		mMeshData->Render();
	}

	void HeightMap::FreeRawImage()
	{
		if (mRawImageData) delete mRawImageData;
		mRawImageData = 0;
	}
}