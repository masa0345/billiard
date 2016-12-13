#pragma once

#include "vector2.h"

typedef Vector2<int> Point;

// マウス入力 singleton
class MouseInput {
public:
	MouseInput(const MouseInput&) = delete;
	MouseInput& operator=(const MouseInput&) = delete;
	MouseInput(MouseInput&&) = delete;
	MouseInput& operator=(MouseInput&&) = delete;
	static void Init();
	static void Dest();
	static MouseInput& GetInstance();

	// マウスのボタンの状態を更新
	void ButtonUpdate();
	// マウスポインタの座標取得
	Point GetPoint() const;
	// マウスホイールの回転量
	int GetWheel() const;
	// クリックが有効になるか
	bool GetValid() const;
	void SetValid(bool valid);

	// 左クリックの状態取得
	// 押した瞬間true
	bool LeftDown() const;
	// 離した瞬間true
	bool LeftUp() const;
	// 押してる間true
	bool LeftDrag() const;
	// マウスドラッグの距離
	vec2f LeftDragDistance() const;

	// 右クリックの状態取得
	// 押した瞬間true
	bool RightDown() const;
	// 離した瞬間true
	bool RightUp() const;
	// 押してる間true
	bool RightDrag() const;
	// マウスドラッグの距離
	vec2f RightDragDistance() const;
	// 現在の位置をclicked_point_right_に入れる
	void UpdateClickRight();
	
private:
	MouseInput() = default;
	~MouseInput() = default;
	static MouseInput* instance_;

	int button_state_, prev_state_;
	int wheel_vol_;
	Point clicked_point_left_, clicked_point_right_;
	Point cur_point_;
	bool valid_;
};