#pragma once

class Ball;
class Table;

// �Փ˔�����s���I�u�W�F�N�g�̃C���^�[�t�F�[�X
class Collidable
{
public:
	// ���菈�����I�������true
	virtual bool CollideWith(Collidable*) = 0;
	virtual bool Response(Ball*) { return true; }
	virtual bool Response(Table*) { return true; }
	// �����W��
	virtual float GetRestitution() const { return 1.f; }
};