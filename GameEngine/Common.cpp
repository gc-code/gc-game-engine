#include <sstream>
#include "Renderer.h"
#include "GUIHandler.h"
#include "Console.h"
#include "Camera.h"
#include "Common.h"

using namespace Renderer;

EngineCommon::EngineCommon()
{
	mRenderManager = new Renderer::RenderManager();
}

EngineCommon::~EngineCommon()
{
	Free();
}

bool EngineCommon::Init(int nCmdShow)
{
	mRenderManager = new RenderManager();
	mRenderSystem = RenderManager::GetRenderSystem();

	System::Keyboard* keyboard = NULL;

	Mechanic::Console* sysConsole = new Mechanic::Console();
	if (!sysConsole->Init())
		return false;

	try
	{
		mTestModel = new MDLModel();
		mGUIHandler = new GUIHandler();

		HRESULT hr = S_OK;

		if (!mRenderSystem->InitWindow("Game Engine", 1000, 600))
			throw "ERROR: failed to initialize window";
		if (!mRenderSystem->InitD3DResources())
			throw "ERROR: failed to initialize render system";

		// The scene immediately caches some constant data so
		// has to be initialized after the render system
		mScene = new RenderScene();
		Mechanic::Camera strategyCam(mScene);
		strategyCam.SetCameraOverHead();

		// Output graphics information to the console
		char driverString[32];
		mRenderSystem->GetD3DDriverString(driverString);
		sysConsole->OutputText("Initialized Direct3D\r\n");
		sysConsole->OutputText("  Direct3D driver type: ");
		sysConsole->OutputText(driverString);
		sysConsole->OutputText("\r\n");
		mRenderSystem->GetD3DFeatureLevelString(driverString);
		sysConsole->OutputText("  Direct3D feature level: ");
		sysConsole->OutputText(driverString);
		sysConsole->OutputText("\r\n\r\n");

		int numCores = mRenderSystem->GetNumPhysicalProcessors();
		std::string coreStr = static_cast<std::ostringstream*>(&(std::ostringstream() << numCores))->str();
		sysConsole->OutputText("Number of physical processors: ");
		sysConsole->OutputText(coreStr.c_str());
		sysConsole->OutputText("\r\n");

		// Get the keyboard state object from the render system
		keyboard = mRenderSystem->GetNativeKeyboard();

		if (!mTestModel->Load("rifle.mdl"))
			throw "ERROR: failed to read MDL model\r\n";
		mScene->AddModel(mTestModel);

		mWorldEditorMap = new HeightMap();
		if (!mWorldEditorMap->LoadMapFromImage("HeightMap.tga"))
			throw "ERROR: failed to load map";
		Material *material = new Material();
		material->SetDiffuseMapPath("grass.tga");
		material->CreateTextures();

		mWorldEditorMap->SetDefaultMatrial(material);

		mScene->SetWorldMap(mWorldEditorMap);

		sysConsole->OutputText("Loaded game resources\r\n");

		if (!mScene->CacheData())
			throw "ERROR: failed to GPU cache data\r\n";

		RenderManager::GetShaderManager()->BindShaderSet(ShaderManager::SHADER_TEXTURE);

		mRenderSystem->DisplayWindow(nCmdShow);
	}
	catch (const char* msg)
	{
		sysConsole->OutputText(msg);
		sysConsole->ErrorSpin();
		return false;
	}

	return true;
}

void EngineCommon::ProcessEvents()
{
	if (mKeyboard->IsKeyPressed(System::Keyboard::KEY_LEFT))
	{
		mTestModel->RotateWorldY(-0.003f);
	}
	else if (mKeyboard->IsKeyPressed(System::Keyboard::KEY_RIGHT))
	{
		mTestModel->RotateWorldY(0.003f);
	}
	else if (mKeyboard->IsKeyPressed(System::Keyboard::KEY_UP))
	{
		mTestModel->RotateWorldX(-0.003f);
	}
	else if (mKeyboard->IsKeyPressed(System::Keyboard::KEY_DOWN))
	{
		mTestModel->RotateWorldX(0.003f);
	}
}

void EngineCommon::RenderFrame()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	mRenderSystem->ClearBuffers(ClearColor);

	if (mKeyboard->IsKeyPressed(System::Keyboard::KEY_ESC))
		PostQuitMessage(0);

	if (mGUIHandler->FileEventReceived())
	{
		std::string fileString;
		fileString = mGUIHandler->GetFileString();
		MDLModel* model = new MDLModel();

		if (!model->Load(fileString.c_str()))
			return;

		model->CreateResources(mScene);
		mScene->AddModel(model);
	}

	ProcessEvents();

	mScene->Render();
}

void EngineCommon::Free()
{
	delete mScene;
	delete mWorldEditorMap;
	delete mRenderManager;
}