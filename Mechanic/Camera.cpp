#include "Renderer.h"
#include "Camera.h"

namespace Mechanic
{
	Camera::Camera(Renderer::RenderScene* scene)
		: mAttachedScene(scene)
	{}

	Camera::~Camera()
	{}

	void Camera::SetCameraOverHead()
	{
		XMMATRIX cameraMat;
		XMVECTOR cameraPosition = XMVectorSet(0.0f, -30.0f, 1.0f, -6.0f);
		XMVECTOR upPosition = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR focus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		
		cameraMat = XMMatrixLookAtLH(cameraPosition, focus, upPosition);
		cameraMat = XMMatrixTranspose(cameraMat);

		XMStoreFloat3(&mCameraPosition, cameraPosition);
		XMStoreFloat3(&mCameraFocus, focus);
		XMStoreFloat4x4(&mCameraMatrix, cameraMat);

		mAttachedScene->SetViewMatrix(&mCameraMatrix);
	}
}