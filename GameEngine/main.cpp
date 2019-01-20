#include "Renderer.h"
#include "GUIHandler.h"
#include "Common.h"

using namespace Renderer;

MDLModel* model = NULL;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	EngineCommon common;
	if (!common.Init(nCmdShow))
		return 0;

	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			common.RenderFrame();
		}
	}

	return (int)msg.wParam;
}
