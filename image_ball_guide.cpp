#include "image_ball_guide.h"
#include "scene.h"
#include "ball.h"
#include "table.h"
#include "range.h"
#include "graphics.h"

struct ImageBallGuide::BounceData
{
	BounceData(const vec3f& p, const vec3f& d, float l) : pos_(p), dir_(d), length_(l) {}
	vec3f pos_, dir_;
	float length_;
};

ImageBallGuide::ImageBallGuide(const vec3f& pos, const vec3f& dir) : 
	GameObject(UI, pos), radius_(0.36f), dir_(dir), length_(20.f), fall_(false)
{
}

void ImageBallGuide::Update()
{
	my_scene_->RegisterCollider(this);
	// 1ƒtƒŒ[ƒ€‚Ì‚Ý¶‘¶‚·‚é
	exist_ = false;
}

void ImageBallGuide::Draw() const
{
	for (auto& bd : wall_bounce_) {
		Graphics3D::DrawLine(bd.pos_, bd.pos_ + bd.dir_ * bd.length_, 0xffffff);
	}
	if (fall_) return;

	vec3f e = pos_ + dir_ * length_;
	Graphics3D::DrawLine(pos_, e, 0xffffff);

	if (!bounce_dir_.IsZero()) {
		Graphics3D::DrawLine(e, e + bounce_dir_.Normalize() * 2.f, 0xffffff);
	}
	if (!ball_dir_.IsZero()) {
		Graphics3D::DrawLine(ball_pos_, ball_pos_ + ball_dir_ * 2.f, 0x00ffff);
	}
}

bool ImageBallGuide::CollideWith(Collidable* c)
{
	return c->Response(this);
}

bool ImageBallGuide::Response(Ball* ball)
{
	if (ball->GetNumber() == 0) return true;

	bool col = IsCollision(ball->GetPos(), pos_, dir_, length_);
	if (!wall_bounce_.empty()) {
		BounceData& bd = wall_bounce_.back();
		if (IsCollision(ball->GetPos(), bd.pos_, bd.dir_, bd.length_)) {
			col = true;
			wall_bounce_.pop_back();
			fall_ = false;
		}
	}
	if (col && !fall_) {
		vec3f col_pos = pos_ + dir_ * length_;
		ball_pos_ = ball->GetPos();
		ball_dir_ = (ball_pos_ - col_pos).Normalize();

		float cross_y = dir_.Cross(pos_ - ball_pos_).y;
		if (cross_y < -0.1f) {
			bounce_dir_ = vec3f(-ball_dir_.z, 0.f, ball_dir_.x);
		} else if (cross_y > 0.1f) {
			bounce_dir_ = vec3f(ball_dir_.z, 0.f, -ball_dir_.x);
		} else {
			bounce_dir_ = vec3f();
		}
	}
	return true;
}

bool ImageBallGuide::Response(Table* t)
{
	float tw = t->GetWidth();
	Range<float> t_range(0.001f, 1.f);
	vec3f dv = dir_ * length_;
	vec3f dp = pos_ - t->GetPos();
	BounceData bounce(pos_, dir_, length_);

	vec3f ndir = dir_;
	float time;
	if (t_range.Within(time = ( tw - dp.z - radius_) / dv.z) || 
		t_range.Within(time = (-tw - dp.z + radius_) / dv.z)) {
			bounce.length_ = time * length_;
			ndir = vec3f(dir_.x, dir_.y, -dir_.z);
			fall_ = t->IsPocketX(pos_.x + dir_.x * bounce.length_);
	} 
	if (t_range.Within(time = ( tw*2.f - dp.x - radius_) / dv.x) ||
		t_range.Within(time = (-tw*2.f - dp.x + radius_) / dv.x)) {
		if (time * length_ < bounce.length_) {
			bounce.length_ = time * length_;
			ndir = vec3f(-dir_.x, dir_.y, dir_.z);
			fall_ = t->IsPocketZ(pos_.z + dir_.z * bounce.length_);
		}
	}

	if (length_ != bounce.length_) {
		pos_ += bounce.dir_ * bounce.length_;
		dir_ = ndir;
		length_ -= bounce.length_;
		wall_bounce_.push_back(bounce);
		return fall_;
	}

	return true;
}

bool ImageBallGuide::IsCollision(const vec3f& bp, const vec3f& p, const vec3f& d, float l)
{
	vec3f dp = p - bp;
	vec3f dv = d * l;
	float rr = 4.f * radius_ * radius_;

	if (dp.LengthSq() < rr) {
		length_ = 0.f;
		pos_ = p;
		dir_ = d;
		return true;
	}
	float a = dv.LengthSq();
	float b = dp.Dot(dv);
	float c = dp.LengthSq() - rr;
	float judge = b*b - a*c;
	if (judge < 0.f) return false;
	judge = std::sqrtf(judge);
	float col_time = (-b - judge) / a;
	if (Range<float>(0.f, 1.f).Within(col_time)) {
		length_ = l * col_time;
		pos_ = p;
		dir_ = d;
		return true;
	}
	return false;
}
