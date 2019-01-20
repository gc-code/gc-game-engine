#ifndef __CAMERA_H__
#define __CAMERA_H__

namespace Mechanic
{
	class Camera
	{
	public:
		Camera(Renderer::RenderScene* scene);

		virtual ~Camera();

		void SetCameraOverHead();

		XMFLOAT4X4 GetCameraMatrix() { return mCameraMatrix; }
		XMFLOAT4X4* GetCameraMatrixPtr() { return &mCameraMatrix; }

		XMFLOAT3 GetCameraPosition() { return mCameraPosition; }

	private:
		XMFLOAT3 mCameraPosition;
		XMFLOAT3 mCameraFocus;
		XMFLOAT4X4 mCameraMatrix;

		Renderer::RenderScene* mAttachedScene;
	};
}

#endif