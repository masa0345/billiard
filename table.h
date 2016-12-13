#pragma once

#include "game_object.h"
#include "collidable.h"
#include <memory>

class Model;

class Table : public GameObject, public Collidable
{
public:
	explicit Table(const vec3f& pos);
	virtual ~Table() = default;

	virtual void Update() override;
	virtual void Draw() const override;

	virtual bool CollideWith(Collidable* c) override;
	virtual float GetRestitution() const override;

	float GetWidth() const;
	float GetHeight() const;
	int GetBallState(int num) const;
	void SetBallState(int num, int state);

	// �|�P�b�g�̂�����W�͈͓̔����`�F�b�N
	bool IsPocketX(float x) const;
	bool IsPocketZ(float z) const;

	// 9�{�[���̏����z�u�ɋ���ݒu��, �Ԃ�l�Ŏ苅��Ԃ�
	Ball* SetupNineBall();

private:
	const float width_; // ��̒Z�ӂ̕�
	const float height_; // �ꂩ�烉�V���������Ă���ʂ܂ł̍���
	std::shared_ptr<Model> model_;
	int ball_states_[16]; // 0:���� 1:��̏� -1:���g�p
};