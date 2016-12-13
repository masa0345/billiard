#pragma once

#include "vector2.h"

typedef Vector2<int> Point;

// �}�E�X���� singleton
class MouseInput {
public:
	MouseInput(const MouseInput&) = delete;
	MouseInput& operator=(const MouseInput&) = delete;
	MouseInput(MouseInput&&) = delete;
	MouseInput& operator=(MouseInput&&) = delete;
	static void Init();
	static void Dest();
	static MouseInput& GetInstance();

	// �}�E�X�̃{�^���̏�Ԃ��X�V
	void ButtonUpdate();
	// �}�E�X�|�C���^�̍��W�擾
	Point GetPoint() const;
	// �}�E�X�z�C�[���̉�]��
	int GetWheel() const;
	// �N���b�N���L���ɂȂ邩
	bool GetValid() const;
	void SetValid(bool valid);

	// ���N���b�N�̏�Ԏ擾
	// �������u��true
	bool LeftDown() const;
	// �������u��true
	bool LeftUp() const;
	// �����Ă��true
	bool LeftDrag() const;
	// �}�E�X�h���b�O�̋���
	vec2f LeftDragDistance() const;

	// �E�N���b�N�̏�Ԏ擾
	// �������u��true
	bool RightDown() const;
	// �������u��true
	bool RightUp() const;
	// �����Ă��true
	bool RightDrag() const;
	// �}�E�X�h���b�O�̋���
	vec2f RightDragDistance() const;
	// ���݂̈ʒu��clicked_point_right_�ɓ����
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