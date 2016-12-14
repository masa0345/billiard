#include "graphics.h"
#include <list>
#include <vector>
#include <memory>
#include <cassert>
#include <DxLib.h>

namespace {
	std::vector<int> fonts;
}

void Graphics3D::DrawLine(const vec3f& s, const vec3f& e, unsigned color)
{
	DxLib::DrawLine3D(VGet(s.x, s.y, s.z), VGet(e.x, e.y, e.z), color);
}

// 2D描画オブジェクト
class DrawObject
{
public:
	virtual void Draw() const = 0;
};
class StringDObject : public DrawObject
{
public:
	StringDObject(int x, int y, char* str, unsigned color) :
		x(x), y(y), str(str), color(color) {}
	virtual void Draw() const override {
		DxLib::DrawString(x, y, str, color);
	}
private:
	int x, y;
	char* str;
	unsigned color;
};
class FontStringDObject : public DrawObject
{
public:
	FontStringDObject(int num, int x, int y, char* str, unsigned color) :
		num(num), x(x), y(y), str(str), color(color) {}
	virtual void Draw() const override {
		assert(static_cast<size_t>(num) < fonts.size());
		DxLib::DrawStringToHandle(x, y, str, color, fonts[num]);
	}
private:
	int num, x, y;
	char* str;
	unsigned color;
};
class RectDObject : public DrawObject
{
public:
	RectDObject(int x1, int y1, int x2, int y2, unsigned color, bool fill, int alpha) :
		x1(x1), y1(y1), x2(x2), y2(y2), color(color), fill(fill), alpha(alpha) {}
	virtual void Draw() const override {
		if (alpha < 255) SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DxLib::DrawBox(x1, y1, x2, y2, color, fill);
		if (alpha < 255) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
private:
	int x1, y1, x2, y2, alpha;
	unsigned color;
	bool fill;
};

static std::list<std::unique_ptr<DrawObject>> buffer;

void Graphics2D::DrawString(int x, int y, char * str, unsigned color)
{
	buffer.push_back(std::make_unique<StringDObject>(x, y, str, color));
}

void Graphics2D::DrawStringCenter(int x, int y, char * str, unsigned color)
{
	int w = GetDrawStringWidth(str, std::strlen(str));
	int h = 16;
	DrawString(x - w/2, y - h/2, str, color);
}

void Graphics2D::DrawFontString(int num, int x, int y, unsigned color, char * str)
{
	buffer.push_back(std::make_unique<FontStringDObject>(num, x, y, str, color));
}

void Graphics2D::DrawFontStringCenter(int num, int x, int y, unsigned color, char * str)
{
	int w = Font::GetWidth(num, str);
	int h = Font::GetSize(num);
	DrawFontString(num, x - w / 2, y - h / 2, color, str);
}

void Graphics2D::DrawRect(int x1, int y1, int x2, int y2, unsigned color, bool fill, int alpha)
{
	buffer.push_back(std::make_unique<RectDObject>(x1, y1, x2, y2, color, fill, alpha));
}

void Graphics2D::DrawRect(float x1, float y1, float x2, float y2, unsigned color, bool fill, int alpha)
{
	DrawRect(static_cast<int>(x1), static_cast<int>(y1), 
		static_cast<int>(x2), static_cast<int>(y2), color, fill, alpha);
}

void Graphics2D::Update()
{
	for (auto& d : buffer) d->Draw();
	buffer.clear();
}

void Font::Init()
{
	fonts.push_back(CreateFontToHandle(nullptr, -1, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4));
	fonts.push_back(CreateFontToHandle(nullptr, 40, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4));
	fonts.push_back(CreateFontToHandle("メイリオ", 64, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4));
	fonts.push_back(CreateFontToHandle("メイリオ", 32, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4));
}

int Font::GetSize(int num)
{
	assert(static_cast<size_t>(num) < fonts.size());
	return GetFontSizeToHandle(fonts[num]);
}

int Font::GetWidth(int num, char * str)
{
	assert(static_cast<size_t>(num) < fonts.size());
	return GetDrawStringWidthToHandle(str, std::strlen(str), fonts[num]);
}
