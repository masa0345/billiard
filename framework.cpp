#include "framework.h"
#include "scene.h"
#include "camera.h"
#include "mouse_input.h"
#include <DxLib.h>

Framework::Framework()
{
	auto size = Camera3D::GetScreenSize();
	ChangeWindowMode(TRUE);
	SetGraphMode(size.x, size.y, 32);
	SetMainWindowText("billiard");
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE_4X4);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);
	// Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);

	MouseInput::Init();
	root_ = new Root();
	root_->Init();
}

Framework::~Framework()
{
	if (root_) delete root_;
	MouseInput::Dest();

	DxLib_End();
}

void Framework::MainLoop()
{

	float x, z;
	x = z = 0.f;

	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !clsDx())
	{
		//Sleep(1000);
		//WaitKey();
		if (CheckHitKey(KEY_INPUT_LEFT)) {
			x -= 0.1f;
		} else if (CheckHitKey(KEY_INPUT_RIGHT)) {
			x += 0.1f;
		}
		if (CheckHitKey(KEY_INPUT_UP)) {
			z += 0.1f;
		} else if (CheckHitKey(KEY_INPUT_DOWN)) {
			z -= 0.1f;
		}
		//DrawLine3D(VGet(x, -2.f, z), VGet(x, 2.f, z), 0xff0000);
		//printfDx("%f %f\n", x, z);

		MouseInput::GetInstance().ButtonUpdate();
		Scene* next_scene = root_->Update();
		if (root_ != next_scene) {
			delete root_;
			root_ = next_scene;
			if(root_) root_->Init();
		}
		if (root_ == nullptr) break;
	}
}
