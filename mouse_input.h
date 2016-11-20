#pragma once

#include "vector2.h"

typedef Vector2<int> Point;

class MouseInput {
public:
	MouseInput();

	// マウスのボタンの状態を更新
	void ButtonUpdate();
	// マウスポインタの座標取得
	Point GetPoint() const;

	// 左クリックの状態取得
	// 押した瞬間true
	bool LeftDown() const;
	// 離した瞬間true
	bool LeftUp() const;
	// 押してる間true
	bool LeftDrag() const;
	// マウスドラッグの距離
	vec2f LeftDragDistance();
	
private:
	int button_state_, prev_state_;
	Point clicked_point_;
};