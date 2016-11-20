#include "mouse_input.h"
#include <DxLib.h>

MouseInput::MouseInput()
{
	prev_state_ = button_state_ = GetMouseInput();
}

void MouseInput::ButtonUpdate()
{
	prev_state_ = button_state_;
	button_state_ = GetMouseInput();
}

Point MouseInput::GetPoint() const
{
	Point p;
	GetMousePoint(&p.x, &p.y);
	return p;
}

bool MouseInput::LeftDown() const
{
	return !(prev_state_&MOUSE_INPUT_LEFT) && (button_state_&MOUSE_INPUT_LEFT);
}

bool MouseInput::LeftUp() const
{
	return (prev_state_&MOUSE_INPUT_LEFT) && !(button_state_&MOUSE_INPUT_LEFT);
}

bool MouseInput::LeftDrag() const
{
	return button_state_ & MOUSE_INPUT_LEFT;
}

vec2f MouseInput::LeftDragDistance()
{
	// �������u�Ԃ̍��W���擾��, �����Ă�ԃh���b�O������Ԃ�
	if (GetMouseInputLog(&button_state_, &clicked_point_.x, &clicked_point_.y) == 0 ||
		LeftDrag()) {
		return GetPoint() - clicked_point_;
	}
	return vec2f();
}
