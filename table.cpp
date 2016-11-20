#include "table.h"
#include "model.h"
#include "scene.h"
#include "ball.h"

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

bool Table::Response(Ball* ball)
{
	// ŒŠ‚É—Ž‚¿‚é”»’è

	return true;
}

float Table::GetRestitution() const
{
	return 0.9f;
}

float Table::GetWidth() const
{
	return width_;
}

int Table::GetBallState(int num) const
{
	return ball_states_[num];
}

Ball* Table::SetupNineBall()
{
	for (int i = 10; i < 16; ++i) ball_states_[i] = -1;
	int nums[] = { 1, 2, 3, 4, 9, 5, 6, 7 ,8 }; // ‚Æ‚è‚ ‚¦‚¸ˆÊ’u‚ÍŒÅ’è
	const vec3f ofs = vec3f(0.625f, 0.f, -0.362f);
	vec3f ini_pos = pos_ + vec3f(width_, height_, 0.f);
	Ball* ball = new Ball(-ini_pos, 0);
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
