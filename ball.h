#pragma once

#include "game_object.h"
#include "collidable.h"
#include <memory>
#include <list>

class Model;

// ボールクラス
class Ball : public GameObject, public Collidable
{
	friend class ColDataIterator;
public:
	Ball(const vec3f& pos, int num);
	virtual ~Ball() = default;

	virtual void Update() override;
	virtual void UpdatePosition() override;
	virtual void Draw() const override;

	virtual bool CollideWith(Collidable* c) override;
	virtual bool Response(Ball* b) override;
	virtual bool Response(Table* t) override;
	virtual bool IsStatic() const override;

	float GetRadius() const;
	int GetNumber() const;
	const vec3f& GetVel() const;
	void SetVel(const vec3f& vel);
	bool IsFalling() const;
	void Reset(const vec3f& pos);

private:
	struct ColData;
	// 衝突情報を追加
	void AddColList(float time, const vec3f& vel, Ball* another, bool fall = false);
	void RemoveColList(float time);
	bool Contains(float time, const vec3f& vel, Ball* another);

	std::shared_ptr<Model> model_; // 3Dモデル
	const float radius_; // 半径
	int number_; // ボールの番号
	vec3f vel_; // 速度
	std::list<ColData> col_;
	bool remove_flag_;
	int falling_;
};