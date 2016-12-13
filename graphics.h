#pragma once

#include "vector3.h"

class Graphics3D
{
public:
	static void DrawLine(const vec3f& s, const vec3f& e, unsigned color);
};

// ���̃N���X��Draw�n�֐���
// Update���Ăяo�������_�ł܂Ƃ߂ĕ`�悳���
class Graphics2D
{
public:
	static void DrawString(int x, int y, char* str, unsigned color);
	static void DrawRect(int x1, int y1, int x2, int y2, unsigned color, bool fill, int alpha = 255);
	static void DrawRect(float x1, float y1, float x2, float y2, unsigned color, bool fill, int alpha = 255);

	static void Update();
};