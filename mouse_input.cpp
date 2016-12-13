#include "mouse_input.h"
#include <cassert>
#include <DxLib.h>

MouseInput* MouseInput::instance_ = nullptr;

void MouseInput::Init()
{
	assert(instance_ == nullptr);
	instance_ = new MouseInput();
	instance_->prev_state_ = instance_->button_state_ = GetMouseInput();
	GetMousePoint(&instance_->cur_point_.x, &instance_->cur_point_.y);
	instance_->wheel_vol_ = GetMouseWheelRotVol();
	instance_->valid_ = true;
}

void MouseInput::Dest()
{
	assert(instance_ != nullptr);
	delete instance_;
}

MouseInput& MouseInput::GetInstance()
{
	assert(instance_);
	return *instance_;
}

void MouseInput::ButtonUpdate()
{
	valid_ = true;
	prev_state_ = button_state_;
	button_state_ = GetMouseInput();
	wheel_vol_ = GetMouseWheelRotVol();
	GetMousePoint(&cur_point_.x, &cur_point_.y);
	if (LeftDown()) {
		clicked_point_left_ = cur_point_;
	}
	if (RightDown()) {
		clicked_point_right_ = cur_point_;
	}
}

Point MouseInput::GetPoint() const
{
	return cur_point_;
}

int MouseInput::GetWheel() const
{
	return wheel_vol_;
}

bool MouseInput::GetValid() const
{
	return valid_;
}

void MouseInput::SetValid(bool valid)
{
	valid_ = valid;
}

bool MouseInput::LeftDown() const
{
	if (!valid_) return false;
	return !(prev_state_&MOUSE_INPUT_LEFT) && (button_state_&MOUSE_INPUT_LEFT);
}

bool MouseInput::LeftUp() const
{
	if (!valid_) return false;
	return (prev_state_&MOUSE_INPUT_LEFT) && !(button_state_&MOUSE_INPUT_LEFT);
}

bool MouseInput::LeftDrag() const
{
	return !LeftDown() && button_state_ & MOUSE_INPUT_LEFT;
}

vec2f MouseInput::LeftDragDistance() const
{
	return cur_point_ - clicked_point_left_;
}

bool MouseInput::RightDown() const
{
	if (!valid_) return false;
	return !(prev_state_&MOUSE_INPUT_RIGHT) && (button_state_&MOUSE_INPUT_RIGHT);
}

bool MouseInput::RightUp() const
{
	if (!valid_) return false;
	return (prev_state_&MOUSE_INPUT_RIGHT) && !(button_state_&MOUSE_INPUT_RIGHT);
}

bool MouseInput::RightDrag() const
{
	return !RightDown() && button_state_ & MOUSE_INPUT_RIGHT;
}

vec2f MouseInput::RightDragDistance() const
{
	return cur_point_ - clicked_point_right_;
}

void MouseInput::UpdateClickRight()
{
	clicked_point_right_ = cur_point_;
}
