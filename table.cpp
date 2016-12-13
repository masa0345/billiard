#include "table.h"
#include "model.h"
#include "scene.h"
#include "ball.h"
#include <algorithm>
#include <random>

Table::Table(const vec3f& pos) : GameObject(TABLE, pos), width_(7.95f), height_(9.4f)
{
	model_ = std::make_shared<Model>("table/pool_table.pmd");
	model_->SetPosition(pos_);
	for (int i = 0; i < 16; ++i) ball_states_[i] = -1;
}


void Table::Update()
{
	Scene::RegisterCollider(this);
	model_->SetPosition(pos_);
}

void Table::Draw() const
{
	model_->Draw();
}

bool Table::CollideWith(Collidable* c)
{
	return c->Response(this);
}

float Table::GetRestitution() const
{
	return 0.9f;
}

float Table::GetWidth() const
{
	return width_;
}

float Table::GetHeight() const
{
	return height_;
}

int Table::GetBallState(int num) const
{
	return ball_states_[num];
}

void Table::SetBallState(int num, int state)
{
	ball_states_[num] = state;
}

bool Table::IsPocketX(float x) const
{
	x -= pos_.x;
	return x < -14.8f || (-0.9f < x && x < 0.9f) || 14.8f < x;
}

bool Table::IsPocketZ(float z) const
{
	z -= pos_.z;
	return z < -6.8f || 6.8f < z;
}

Ball* Table::SetupNineBall()
{
	for (int i = 10; i < 16; ++i) ball_states_[i] = -1;
	//int nums[] = { 1, 2, 3, 4, 9, 5, 6, 7 ,8 }; // �ʒu�Œ�
	// �I���̈ʒu�̓����_��
	std::vector<int> nums{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::random_device rd;
	std::shuffle(nums.begin(), nums.end(), std::mt19937(rd()));
	for (unsigned i = 0; i < nums.size(); ++i) {
		// 1��9�͌Œ�
		if (nums[i] == 1) std::swap(nums[i], nums[0]);
		if (nums[i] == 9) std::swap(nums[i], nums[4]);
	}
	const vec3f ofs = vec3f(0.625f, 0.f, -0.362f);
	vec3f ini_pos = pos_ + vec3f(width_, height_, 0.f);
	Ball* ball = new Ball(vec3f(pos_.x - width_, pos_.y + height_, pos_.z - width_*0.5f), 0);
	ball_states_[0] = 1;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			new Ball(ini_pos + ofs*static_cast<float>(j), nums[i * 3 + j]);
			ball_states_[i * 3 + j + 1] = 1;
		}
		ini_pos.x += ofs.x;
		ini_pos.z -= ofs.z;
	}
	return ball;
}
