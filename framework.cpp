#include "framework.h"
#include "scene.h"
#include <DxLib.h>

Framework::Framework()
{
	const int WindowWidth = 640, WindowHeight = 480;

	ChangeWindowMode(TRUE);
	SetGraphMode(WindowWidth, WindowHeight, 32);
	SetMainWindowText("billiard");
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);
	// Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);

	root_ = new Root();
}

Framework::~Framework()
{
	if (root_) delete root_;

	DxLib_End();
}

void Framework::MainLoop()
{
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !clsDx())
	{
		//Sleep(1000);
		//WaitKey();

		printfDx("キューの方向に手球を飛ばします\n");
		printfDx("穴には落ちません\n");

		Scene* next_scene = root_->Update();
		if (root_ != next_scene) {
			delete root_;
			root_ = next_scene;
		}
		if (root_ == nullptr) break;
	}
}
