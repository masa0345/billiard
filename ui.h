#pragma once

#include "game_object.h"
#include <string>

// 2DのUI
class PlaneUI : public GameObject
{
public:
	PlaneUI(int x, int y);
	virtual ~PlaneUI() = default;
	virtual void Update() override;
	virtual void Draw() const override = 0;
protected:
	int x_, y_;
};

// ボタン
class Button : public PlaneUI
{
public:
	Button(int x, int y, int w, int h, const char* str = nullptr);
	virtual void Draw() const override;

	bool IsClicked() const;
	bool IsMouseOver() const;

protected:
	int width_, height_;
	std::string str_;
};

// クリックできる文字列
class StringButton : public Button
{
public:
	StringButton(int x, int y, int font_num, const char* str);
	virtual void Draw() const override;
private:
	int font_num_;
};

// チェックボックス
class CheckBox : public PlaneUI
{
public:
	CheckBox(int x, int y, bool check);
	virtual void Update() override;
	virtual void Draw() const override;
	bool GetCheck() const;

private:
	bool check_;
	int size_;
};

// スライダー
class Slider : public PlaneUI
{
public:
	Slider(int x, int y, int val, int length);
	virtual void Update() override;
	virtual void Draw() const override;
	int GetVal() const;

private:
	int val_, length_;
	int size_;
};

class Player;

// ポーズ画面のメニュー
class PauseMenu : public PlaneUI
{
public:
	PauseMenu(Player* p);
	virtual void Update() override;
	virtual void Draw() const override;

	bool PushClose() const;
	bool PushRestart() const;
	bool PushTitle() const;

private:
	Player* player_;
	Button* close_;
	CheckBox* guide_;
	Slider* bgm_volume_;
	Slider* se_volume_;
	Button* restart_;
	Button* title_;
};