#pragma once

#include "vector2.h"

typedef Vector2<int> Point;

class MouseInput {
public:
	MouseInput();

	// �}�E�X�̃{�^���̏�Ԃ��X�V
	void ButtonUpdate();
	// �}�E�X�|�C���^�̍��W�擾
	Point GetPoint() const;

	// ���N���b�N�̏�Ԏ擾
	// �������u��true
	bool LeftDown() const;
	// �������u��true
	bool LeftUp() const;
	// �����Ă��true
	bool LeftDrag() const;
	// �}�E�X�h���b�O�̋���
	vec2f LeftDragDistance();
	
private:
	int button_state_, prev_state_;
	Point clicked_point_;
};