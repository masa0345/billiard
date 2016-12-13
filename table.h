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

	// ポケットのある座標の範囲内かチェック
	bool IsPocketX(float x) const;
	bool IsPocketZ(float z) const;

	// 9ボールの初期配置に球を設置し, 返り値で手球を返す
	Ball* SetupNineBall();

private:
	const float width_; // 台の短辺の幅
	const float height_; // 底からラシャが張ってある面までの高さ
	std::shared_ptr<Model> model_;
	int ball_states_[16]; // 0:落下 1:台の上 -1:未使用
};