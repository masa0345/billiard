#include <DxLib.h>
#include "framework.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	//ƒƒOo—Í
	SetOutApplicationLogValidFlag(TRUE);
#else
	SetOutApplicationLogValidFlag(FALSE);
#endif

	Framework* game = new Framework();
	game->MainLoop();
	delete game;

	return 0;
}