#pragma once

#include "vector3.h"
#include <string>

class Graphics3D
{
public:
	static void DrawLine(const vec3f& s, const vec3f& e, unsigned color);
};

// このクラスのDraw系関数は
// Updateを呼び出した時点でまとめて描画される
class Graphics2D
{
public:
	static void DrawString(int x, int y, const std::string& str, unsigned color);
	static void DrawStringCenter(int x, int y, const std::string& str, unsigned color);
	static void DrawFontString(int num, int x, int y, unsigned color, const std::string& str);
	static void DrawFontStringCenter(int num, int x, int y, unsigned color, const std::string& str);
	static void DrawRect(int x1, int y1, int x2, int y2, unsigned color, bool fill, int alpha = 255);
	static void DrawRect(float x1, float y1, float x2, float y2, unsigned color, bool fill, int alpha = 255);

	static void Update();
};

class Font
{
public:
	static void Init();
	static int GetSize(int num);
	static int GetWidth(int num, const char* str);
};