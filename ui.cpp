#include "ui.h"
#include "mouse_input.h"
#include "graphics.h"
#include "range.h"
#include "player.h"
#include "sound.h"
#include <cassert>

PlaneUI::PlaneUI(int x, int y) : 
	GameObject(UI, vec3f(static_cast<float>(x), static_cast<float>(y), 0.f)), x_(x), y_(y) {}

void PlaneUI::Update() {}

Button::Button(int x, int y, int w, int h, const char* str) : 
	PlaneUI(x, y), width_(w), height_(h), str_(str)
{
}

void Button::Draw() const
{
	unsigned color = IsMouseOver() ? 0xffffff : 0xdddddd;
	Graphics2D::DrawRect(x_, y_, x_ + width_, y_ + height_, color, true);
	if(!str_.empty()) Graphics2D::DrawString(x_ + 4, y_ + height_/4, str_, 0xffffff);
}

bool Button::IsClicked() const
{
	return MouseInput::GetInstance().LeftDown() && IsMouseOver();
}

bool Button::IsMouseOver() const
{
	auto mouse_pos = MouseInput::GetInstance().GetPoint();
	return Range<int>(x_, x_ + width_).WithinEq(mouse_pos.x) &&
			Range<int>(y_, y_ + height_).WithinEq(mouse_pos.y);
}


StringButton::StringButton(int x, int y, int font_num, const char* str) :
	Button(x, y, Font::GetWidth(font_num, str), Font::GetSize(font_num), str), font_num_(font_num)
{
}

void StringButton::Draw() const
{
	unsigned color = IsMouseOver() ? 0xffffff : 0xaaaaaa;
	Graphics2D::DrawFontString(font_num_, x_, y_, color, str_);
}


CheckBox::CheckBox(int x, int y, bool check) : PlaneUI(x, y), check_(check), size_(20)
{
}

void CheckBox::Update()
{
	auto mouse_pos = MouseInput::GetInstance().GetPoint();
	if (MouseInput::GetInstance().LeftDown() &&
		Range<int>(x_, x_ + size_).WithinEq(mouse_pos.x) &&
		Range<int>(y_, y_ + size_).WithinEq(mouse_pos.y)) {
		check_ = !check_;
	}
}

void CheckBox::Draw() const
{
	unsigned color = check_ ? 0xffffff : 0x777777;
	Graphics2D::DrawRect(x_, y_, x_ + size_, y_ + size_, color, true);
	Graphics2D::DrawString(x_+4, y_+2, check_ ? "T" : "F", color);
}

bool CheckBox::GetCheck() const
{
	return check_;
}

Slider::Slider(int x, int y, int val, int length) : 
	PlaneUI(x, y), val_(val), length_(length), size_(20)
{
	assert(val_ <= length_);
}

void Slider::Update()
{
	auto mouse_pos = MouseInput::GetInstance().GetPoint();
	if (MouseInput::GetInstance().LeftDrag() &&
		Range<int>(x_, x_ + length_).WithinEq(mouse_pos.x) &&
		Range<int>(y_, y_ + size_).WithinEq(mouse_pos.y)) {
		val_ = mouse_pos.x - x_;
	}
}

void Slider::Draw() const
{
	Graphics2D::DrawRect(x_, y_ + size_ / 4, x_ + length_ + size_ / 2, y_ + size_ * 3 / 4, 0x777777, true);
	Graphics2D::DrawRect(x_ + val_, y_, x_ + val_ + size_ / 2, y_ + size_, 0xffffff, true);
}

int Slider::GetVal() const
{
	return val_;
}

PauseMenu::PauseMenu(Player* p) : PlaneUI(220, 110), player_(p)
{
	int slider_len = 100;
	int se = Sound::GetInstance().GetSEVol();
	int bgm = Sound::GetInstance().GetBGMVol();
	se = se * slider_len / 255;
	bgm = bgm * slider_len / 255;
	close_		= new Button(x_ + 170, y_, 30, 30, "閉");
	guide_		= new CheckBox(x_ + 160, y_ + 60, p->GetDisplayGuide());
	bgm_volume_	= new Slider(x_ +  80, y_ + 100, bgm, slider_len);
	se_volume_	= new Slider(x_ +  80, y_ + 140, se, slider_len);
	restart_	= new Button(x_ +  10, y_ + 200, 80, 30, " 再挑戦");
	title_		= new Button(x_ + 110, y_ + 200, 80, 30, "タイトル");
}

void PauseMenu::Update()
{
	player_->SetDisplayGuide(guide_->GetCheck());

	int slider_len = 100;
	int bgm = Sound::GetInstance().GetBGMVol();
	bgm = bgm * slider_len / 255;
	if (bgm_volume_->GetVal() != bgm) {
		Sound::GetInstance().SetBGMVol(bgm_volume_->GetVal() * 255 / slider_len);
	}
	int se = Sound::GetInstance().GetSEVol();
	se = se * slider_len / 255;
	if (se_volume_->GetVal() != se) {
		Sound::GetInstance().SetSEVol(se_volume_->GetVal() * 255 / slider_len);
	}
}

void PauseMenu::Draw() const
{
	Graphics2D::DrawRect(x_, y_, x_ + 200, y_ + 260, 0x444444, true, 60);
	Graphics2D::DrawString(x_ + 20, y_ + 60, "ショット予測線", 0xffffff);
	Graphics2D::DrawString(x_ + 20, y_ + 100, "BGM", 0xffffff);
	Graphics2D::DrawString(x_ + 20, y_ + 140, "SE", 0xffffff);
	Graphics2D::DrawRect(x_, y_, x_ + 200, y_ + 260, 0xffffff, false);
}

bool PauseMenu::PushClose() const
{
	return close_->IsClicked();
}

bool PauseMenu::PushRestart() const
{
	return restart_->IsClicked();
}

bool PauseMenu::PushTitle() const
{
	return title_->IsClicked();
}
