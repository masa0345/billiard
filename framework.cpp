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

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);
	// �y�o�b�t�@�ւ̏������݂�L���ɂ���
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

		printfDx("�L���[�̕����Ɏ苅���΂��܂�\n");
		printfDx("���ɂ͗����܂���\n");

		Scene* next_scene = root_->Update();
		if (root_ != next_scene) {
			delete root_;
			root_ = next_scene;
		}
		if (root_ == nullptr) break;
	}
}
