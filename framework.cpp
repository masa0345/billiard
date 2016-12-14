#include "framework.h"
#include "scene.h"
#include "camera.h"
#include "mouse_input.h"
#include "graphics.h"
#include "sound.h"
#include <DxLib.h>

Framework::Framework()
{
	auto size = Camera3D::GetScreenSize();
	ChangeWindowMode(TRUE);
	SetGraphMode(size.x, size.y, 32);
	SetMainWindowText("billiard");
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	// 3Dサウンドを使用
	SetEnableXAudioFlag(TRUE); 
	Set3DSoundOneMetre(10.f);

	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE_4X4);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);
	// Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);

	Font::Init();
	Sound::Init();
	MouseInput::Init();
	root_ = new Root();
	root_->Init();
}

Framework::~Framework()
{
	if (root_) delete root_;
	MouseInput::Dest();
	Sound::Dest();

	DxLib_End();
}

void Framework::MainLoop()
{
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !clsDx())
	{
		Sound::GetInstance().Update();
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
